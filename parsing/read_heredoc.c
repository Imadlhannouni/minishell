/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 14:31:36 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/04 16:03:34 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*free_line(char *new_value, int i)
{
	static char	*value = NULL;

	if (i != -1)
	{
		if (value)
			free(value);
		value = new_value;
	}
	return (value);
}

void	signalhandler(int signum)
{
	int		i;
	char	*line;

	(void)signum;
	i = global_var2(-1);
	line = free_line(NULL, -1);
	if (line)
		free(line);
	close(i);
	ft_malloc(0, 1);
	write(1, "\n", 1);
	exit(130);
}

static void	child_heredoc_loop(struct s_heredoc *heredoc, char **exit_code,
		char **clone_envi)
{
	char	*line1;
	char	*line;
	int		i;

	i = 0;
	signal(SIGINT, signalhandler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		free_line(line1 = readline("> "), 1);
		i = global_var2(heredoc->write_fd);
		if (!line1)
		{
			ft_putstr_fd("warning: here-document delimited ", 2);
			ft_putstr_fd("by end-of-file (wanted `", 2);
			ft_putstr_fd(heredoc->delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		line = ft_strdup(line1);
		if (ft_strcmp(line, heredoc->delimiter) == 0)
			break ;
		expand_and_write_line(heredoc, line, exit_code, clone_envi);
	}
	free(free_line(NULL, -1));
	close(heredoc->write_fd);
	ft_malloc(0, 1);
	exit(0);
}

static int	handle_parent_status(int status)
{
	int	exit_code;

	exit_code = -1;
	if (WIFSIGNALED(status))
	{
		write(1, "\n", 1);
		exit_code = WTERMSIG(status) + 128;
	}
	else if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSTOPPED(status))
		exit_code = WSTOPSIG(status) + 128;
	return (exit_code);
}

static int	parent_heredoc_read(pid_t pid, char **exit_code)
{
	int		status;
	void	*old_handler;

	status = -1;
	old_handler = signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	status = handle_parent_status(status);
	*exit_code = ft_itoa(status);
	signal(SIGINT, old_handler);
	if (status >= 130)
		return (0);
	return (1);
}

char	*read_heredoc(char *delimiter, char **clone_envi, t_token_type type,
		char **exit_code)
{
	pid_t				pid;
	static int			heredoc_num = 0;
	struct s_heredoc	*heredoc;
	char				*num_str;

	heredoc = (t_heredoc *)ft_malloc(sizeof(struct s_heredoc), 0);
	num_str = ft_itoa(heredoc_num++);
	heredoc->filename = ft_strjoin("/tmp/.heredoc_", num_str);
	heredoc->write_fd = open(heredoc->filename, O_CREAT | O_WRONLY | O_TRUNC,
			0600);
	if (heredoc->write_fd == -1)
		return (putstr_fd("Error When Opening file\n", 2), NULL);
	pid = fork();
	if (pid == -1)
		return (NULL);
	heredoc->delimiter = delimiter;
	heredoc->type = type;
	if (pid == 0)
		child_heredoc_loop(heredoc, exit_code, clone_envi);
	if (!parent_heredoc_read(pid, exit_code))
		return (close(heredoc->write_fd), NULL);
	close(heredoc->write_fd);
	return (heredoc->filename);
}
