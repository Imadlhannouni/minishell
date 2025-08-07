/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:21 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/07 12:51:53 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_var(t_vars *var, size_t pipe_num)
{
	size_t	j;

	j = 0;
	var->i = 0;
	var->pipe_num = pipe_num;
	if (pipe_num > 1)
	{
		var->pid = ft_malloc((pipe_num) * sizeof(__pid_t), 0);
		var->fd = ft_malloc((pipe_num - 1) * sizeof(int [2]), 0);
		while (j < var->pipe_num - 1)
		{
			if (pipe((var->fd)[j]) < 0)
			{
				perror("Minishell");
				return (close_previous(var->fd, j - 1), -1);
			}
			j++;
		}
	}
	return (0);
}

int	extract_status(int status)
{
	int	exit_code;

	exit_code = -1;
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		exit_code = WTERMSIG(status) + 128;
		if (exit_code == 130)
			putstr_fd("\n", 1);
		if (exit_code == 131)
			putstr_fd("Quit (core dumped)\n", 2);
	}
	return (exit_code);
}

static void	exec_helper(char *path, t_exe *tmp, char ***env, t_help *help)
{
	if (handle_redirections(tmp) < 0)
		exit_free(1);
	if (!tmp->arr || !tmp->arr[0])
		exit_free(0);
	if (is_builtin(tmp->arr[0]) == 1)
		exit_free(exec_builtin(tmp, env, help));
	if ((!path || !*path))
	{
		putstr_fd("Minishell : command not found\n", 2);
		exit_free(127);
	}
	if (!is_builtin(tmp->arr[0]))
	{
		execve(path, tmp->arr, *env);
		perror("Minishell");
		exit_free(127);
	}
}	

int	helper(t_exe *tmp, char ***env, t_vars var, t_help *help)
{
	char	*path;
	int		e_code;
	int		pid;

	path = NULL;
	e_code = 0;
	pid = fork();
	if (pid < 0)
		return (perror("Minishell"), -1);
	if (pid == 0)
	{
		help->child = 1;
		close_fd(var.fd, var.i, var.pipe_num);
		switch_fd(var.fd, var.i, var.pipe_num - 1);
		signal(SIGINT, help->prev_handler_int);
		signal(SIGQUIT, help->prev_handler_quit);
		if (!is_builtin(tmp->arr[0]))
			e_code = retrieve_path(tmp->arr[0], *env, &path);
		if (e_code > 0)
			return (check_path(path, &e_code), exit_free(e_code), 0);
		exec_helper(path, tmp, env, help);
	}
	return (pid);
}

int	exec_pipe(t_exe *grp, char ***envp, size_t pipe_num, t_help *help)
{
	t_vars	var;
	int		status;
	int		exit_code;

	if (init_var(&var, pipe_num) < 0)
		return (1);
	status = -1;
	exit_code = -1;
	while (var.i < pipe_num)
	{
		var.pid[var.i] = helper(grp, envp, var, help);
		if (var.pid[var.i++] < 0)
			return (1);
		grp = grp->next;
	}
	close_previous(var.fd, var.pipe_num - 1);
	var.i = 0;
	signal(SIGINT, SIG_IGN);
	waitpid(var.pid[var.pipe_num - 1], &status, 0);
	while (var.i++ < var.pipe_num - 1)
		wait(NULL);
	signal(SIGINT, sighandler);
	exit_code = extract_status(status);
	return (exit_code);
}
