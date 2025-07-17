/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 14:31:36 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/16 21:39:16 by ilhannou         ###   ########.fr       */
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
	struct sigaction	sa;
	struct sigaction	old_sa;
	int					interrupted = 0;
	int					saved_errno;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_sa);
	content = ft_strdup("");
	while (1)
	{
		signal(SIGINT, SIG_DFL);
		errno = 0;
		line = readline("> ");
		saved_errno = errno;
		signal(SIGINT, SIG_IGN);
		if (!line)
        {
            if (saved_errno == EINTR)
            {
                interrupted = 1;
                write(1, "\n", 1);
                break;
            }
            ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `", 2);
            ft_putstr_fd(delimiter, 2);
            ft_putstr_fd("')\n", 2);
            break;
        }
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
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
		tmp = content;
		content = ft_strjoin(tmp, line);
		free(tmp);
		if (!content)
			return(free(content), NULL);
		tmp = content;
		content = ft_strjoin(tmp, "\n");
		free(tmp);
		if (!content)
			return(free(content), NULL);
		free(line);
	}
	sigaction(SIGINT, &old_sa, NULL);
	if (interrupted)
    {
        free(content);
        return (NULL);
    }
	return (content);
}

char	*create_heredoc_file(char *content)
{
	static int	heredoc_num = 0;
	char		*filename;
	int			fd;

	filename = ft_strjoin("/tmp/.heredoc_", ft_itoa(heredoc_num++));
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("Erreur lors de l'ouverture du fichier");
        return (NULL);
	}
	write(fd, content, ft_strlen(content));
	close(fd);
	return (filename);
}
