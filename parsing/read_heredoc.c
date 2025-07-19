/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 14:31:36 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/19 16:34:48 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_delimiter(char *delimiter)
{
	if (ft_strchr(delimiter, '"'))
		return (1);
	return (0);
}

char	*read_heredoc(char *delimiter, char **clone_envi, t_token_type type)
{
	char				*line;
	char				*content;
	char				*tmp;
	int					i;
	char				*expanded_line;
	int					pipefd[2];
	pid_t				pid;
	int					status;
	char				buffer[1024];
	ssize_t				bytes;

	if (pipe(pipefd) == -1)
		return (NULL);

	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (NULL);
	}

	if (pid == 0)
	{
		close(pipefd[0]);
		signal(SIGINT, SIG_DFL);

		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `", 2);
				ft_putstr_fd(delimiter, 2);
				ft_putstr_fd("')\n", 2);
				break;
			}
			if (ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break;
			}
			if (type != 9 && type != 3 && type != 4 && ft_strchr(line, '$'))
			{
				i = 0;
				expanded_line = ft_strdup("");
				while (line[i])
					i = append_env_or_chunk(line, i, clone_envi, &expanded_line);
				free(line);
				line = expanded_line;
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		exit(0);
	}
	close(pipefd[1]);
	signal(SIGINT, SIG_IGN);
	content = ft_strdup("");
	while ((bytes = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes] = '\0';
		tmp = content;
		content = ft_strjoin(tmp, buffer);
		free(tmp);
		if (!content)
		{
			close(pipefd[0]);
			waitpid(pid, &status, 0);
			return (NULL);
		}
	}
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	if (status != 0)
	{
		write(1, "\n", 1);
		free(content);
		return (NULL);
	}
	return (content);
}

char	*create_heredoc_file(char *content)
{
    static int	heredoc_num = 0;
    char		*filename;
    char		*num_str;
    int			fd;

    num_str = ft_itoa(heredoc_num++);
    filename = ft_strjoin("/tmp/.heredoc_", num_str);
    free(num_str);
    if (!filename)
        return (NULL);
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd == -1)
    {
        perror("Erreur lors de l'ouverture du fichier");
        free(filename);
        return (NULL);
    }
    write(fd, content, ft_strlen(content));
    close(fd);
    return (filename);
}

void cleanup_heredoc_files(t_pipe *pipes)
{
    t_pipe *current_pipe = pipes;
    t_token *current_token;

    while (current_pipe)
    {
        current_token = current_pipe->full_cmd;
        while (current_token)
        {
            if (current_token->heredoc == 1 && current_token->value)
                unlink(current_token->value);
            current_token = current_token->next;
        }
        current_pipe = current_pipe->nextpipe;
    }
}
