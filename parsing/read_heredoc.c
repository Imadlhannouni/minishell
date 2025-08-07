/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 14:31:36 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/06 21:14:25 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	putstr_fd("\n", 1);
	exit(130);
}

static void	child_heredoc_loop(struct s_heredoc *heredoc, char **exit_code,
		char **clone_envi)
{
	char	*line1;

	signal(SIGINT, signalhandler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		global_var2(heredoc->write_fd);
		free_line(line1 = readline("> "), 1);
		if (!line1)
		{
			putstr_fd("warning: here-document delimited ", 2);
			putstr_fd("by end-of-file (wanted `", 2);
			putstr_fd(heredoc->delimiter, 2);
			putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strcmp(line1, heredoc->delimiter) == 0)
			break ;
		expand_and_write_line(heredoc, line1, exit_code, clone_envi);
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
		putstr_fd("\n", 1);
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
	struct s_heredoc	*heredoc;
	char				*num_str;
	int					random_id;

	random_id = randomid();
	if (!random_id)
		return (NULL);
	num_str = ft_itoa(random_id);
	heredoc = (t_heredoc *)ft_malloc(sizeof(struct s_heredoc), 0);
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
	return (close(heredoc->write_fd), heredoc->filename);
}
