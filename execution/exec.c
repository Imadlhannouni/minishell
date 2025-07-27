/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:31 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/27 20:32:12 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_command(t_exe *var, char **env, t_free *collect)
{
	int		pid;
	int		status;
	char 	*path;
	int		exit_code;

	status = -1;
	exit_code = -1;
	path = retrieve_path(var->arr[0], env, collect);
	pid = fork();
	if (pid < 0)
		return (putstr_fd("fork() failed\n", 2), 1);
	if (pid == 0)
	{
		if (handle_redirections(var) < 0)
			exit_free(collect, 1);
		if (execve(path, var->arr, env) == -1)
			putstr_fd("Command not found\n",2);
		exit_free(collect, 127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
    	exit_code = WEXITSTATUS(status);
	free(path);
	return (exit_code);
}
void	init_collect(t_free *collect)
{
	collect->env = NULL;
	collect->exe = NULL;
	collect->fd = NULL;
	collect->no_val = NULL;
	collect->pid = NULL;
	collect->pipes = NULL;
	collect->fds = NULL;
	collect->path = NULL;
}

void group_pipes(t_pipe *pipes, t_exe **var, t_free *collect, char **env)
{
	t_pipe *tmp;

	if (!pipes)
		return;
	init_collect(collect);
	collect->pipes = pipes;
	tmp = pipes;
	while (tmp)
	{
		add_node(var,creat_node(tmp->full_cmd));
		tmp = tmp->nextpipe;
	}
	collect->exe = *var;
	collect->env = env;
}

void reset_redirections(int fd[2])
{
	dup2(fd[0] ,STDIN_FILENO);
	dup2(fd[1] ,STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
}


void dup_std(int fd[2], t_free *collect)
{
	fd[0] = dup(STDIN_FILENO);
	fd[1] =	dup(STDOUT_FILENO);
	collect->fds = fd;
}


int	execute(t_pipe *pipes, char ***env)
{
	t_exe	*var;
	t_free	collect;
	int fd[2];
	int status = -1;

	var = NULL;
	group_pipes(pipes, &var, &collect, *env);
	if (count_pipes(pipes) > 1)
		status = exec_pipe(var, env, count_pipes(pipes), &collect);
	else if (count_pipes(pipes) == 1)
	{
		dup_std(fd, &collect);
		if (!is_builtin(var->arr[0]))
		status = exec_command(var, *env, &collect);
		else
		{
			if (handle_redirections(var) < 0)
				return (1);
			if (var->arr)
				status = exec_builtin(var, env, &collect);
		}
		reset_redirections(fd);
	}
	free_t_exe(&var);
	return status;
}
