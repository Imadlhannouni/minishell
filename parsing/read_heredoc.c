/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 14:31:36 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/01 14:03:51 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int g_signal_received = 0;

void signalhandler(int signum)
{
	(void)signum;
	g_signal_received = 1;
	close(STDIN_FILENO);
}

int	check_delimiter(char *delimiter)
{
	if (ft_strchr(delimiter, '"'))
		return (1);
	return (0);
}

static void	expand_and_write_line(int write_fd, char *line, char **clone_envi,
		t_token_type type, char **exit_code)
{
	char	*expanded_line;
	int		i;

	i = 0;
	if (type != 9 && type != 3 && type != 4 && ft_strchr(line, '$'))
	{
		expanded_line = ft_strdup("");
		while (line[i])
			i = append_env_or_chunk(line, i, clone_envi, &expanded_line,
					*exit_code);
		line = expanded_line;
	}
	write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
}

static void	child_heredoc_loop(int write_fd, char *delimiter, char **clone_envi,
		t_token_type type, char **exit_code)
{
	char	*line;

	signal(SIGINT, signalhandler);
	while (1)
	{
		line = readline("> ");
		if(g_signal_received)
		{
			free(line);
			ft_malloc(0, 1);
			close(write_fd);
			open("/dev/tty", O_RDONLY);
			exit(130);
		}
		if (!line)
		{
			ft_putstr_fd("warning: here-document delimited by end-of-file (wanted `",
				2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		expand_and_write_line(write_fd, line, clone_envi, type, exit_code);
	}
	free(line);
	ft_malloc(0, 1);
	close(write_fd);
	exit(0);
}

static int	handle_parent_status(int status, char **exit_code)
{
	if (status != 0)
	{
		write(1, "\n", 1);
		*exit_code = ft_strdup("130");
		return (1);
	}
	return (0);
}

static char	*parent_heredoc_read(int read_fd, pid_t pid, int *status,
		char **exit_code)
{
	ssize_t	bytes;
	char	*content;
	char	*tmp;
	char buffer[4096];

	void *old_handler = signal(SIGINT, SIG_IGN);
	content = ft_strdup("");
	while ((bytes = read(read_fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes] = '\0';
		tmp = content;
		content = ft_strjoin(tmp, buffer);
		if (!content)
		{
			close(read_fd);
			waitpid(pid, status, 0);
			return (NULL);
		}
	}
	close(read_fd);
	signal(SIGINT, old_handler);
	waitpid(pid, status, 0);
	if (handle_parent_status(*status, exit_code))
		return (NULL);
	return (content);
}

char	*read_heredoc(char *delimiter, char **clone_envi, t_token_type type,
		char **exit_code)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

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
		child_heredoc_loop(pipefd[1], delimiter, clone_envi, type, exit_code);
	}
	close(pipefd[1]);
	return (parent_heredoc_read(pipefd[0], pid, &status, exit_code));
}

char	*create_heredoc_file(char *content)
{
	static int	heredoc_num = 0;
	char		*filename;
	char		*num_str;
	int			fd;

	num_str = ft_itoa(heredoc_num++);
	filename = ft_strjoin("/tmp/.heredoc_", num_str);
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		putstr_fd("Error When Opening file\n", 2);
		return (NULL);
	}
	write(fd, content, ft_strlen(content));
	close(fd);
	return (filename);
}

void	cleanup_heredoc_files(t_pipe *pipes)
{
	t_pipe	*current_pipe;
	t_token	*current_token;

	current_pipe = pipes;
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
