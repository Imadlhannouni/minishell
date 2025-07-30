/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:21 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/30 23:22:00 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_var(t_vars *var, size_t pipe_num, t_free *collect)
{
	size_t	j;

	j = 0;
	var->i = 0;
	var->pipe_num = pipe_num;
	if (pipe_num > 1)
	{
		var->pid = malloc((pipe_num) * sizeof(__pid_t));
		if (!var->pid)
			exit_free(collect, 2);
		collect->pid = var->pid;
		var->fd = malloc((pipe_num - 1) * sizeof(int[2]));
		if (!var->fd)
			exit_free(collect, 1);
		collect->fd = var->fd;
		while (j < var->pipe_num - 1)
		{
			if (pipe((var->fd)[j]) == -1)
				return (close_previous(var->fd, j - 1),free(var->fd),free(var->pid),1);
			j++;
		}
	}
	return 0;
}

int is_path1(char *cmd)
{
	int i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return 1;
		i++;
	}
	return 0;
}

int helper(t_exe *tmp ,char ***env, t_vars var, t_free *collect)
{
	char *path = NULL;

	if (!is_builtin(tmp->arr[0]))
		path = retrieve_path(tmp->arr[0], *env, collect);
	int pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		close_fd(var.fd, var.i, var.pipe_num);
		switch_fd(var.fd, var.i, var.pipe_num - 1);
		if (!path && !is_builtin(tmp->arr[0]))
		{
			putstr_fd("Minishell : command not found\n", 2);
			exit_free(collect, 127);
		}
		if (handle_redirections(tmp) < 0)
			exit_free(collect, 2);
		if (!is_builtin(tmp->arr[0]))
		{
			execve(path, tmp->arr, *env);
			perror("Minishell");
			exit_free(collect, 127);
		}
		exit_free(collect, exec_builtin(tmp, env, collect));
	}
	if (path)
		free(path);
	return (path = NULL, pid);
}

int exec_pipe(t_exe *grp, char ***envp, size_t pipe_num, t_free *collect)
{
	t_vars 	var;
	t_exe 	*tmp;
	int		status;
	int		exit_code;

	if (init_var(&var, pipe_num, collect))
		return 1;
	status = -1;
	exit_code = -1;
	tmp = grp;
	while (var.i < pipe_num) 
	{
		var.pid[var.i] = helper(tmp, envp, var, collect);
		if (var.pid[var.i++] < 0)
			return (free(var.pid), free(var.fd), 1);
		tmp = tmp->next;
	}
	close_previous(var.fd, var.pipe_num - 1);
	var.i = 0;
	waitpid(var.pid[var.pipe_num - 1], &status, 0);
	while (var.i++ < var.pipe_num - 1)
		wait(NULL);
	if (WIFEXITED(status))
    	exit_code = WEXITSTATUS(status);
	return (free(var.pid), free(var.fd), var.fd=NULL, var.pid=NULL, exit_code);
}


