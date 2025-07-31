/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:31 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/31 16:24:39 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void help_exec(t_exe *var, t_free *collect, char *path)
{
	if (handle_redirections(var) < 0)
	{
		if (path)
			free(path);
		close(collect->fds[0]);
		close(collect->fds[1]);		
		exit_free(collect, 127);
	}
	if (!path)
	{
		putstr_fd("Minishell : Command not found\n",2);
		reset_redirections(collect->fds);
		exit_free(collect, 127);
	}
}

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
		return (putstr_fd("Minishell : fork() failed\n", 2), free(path), 1);
	if (pid == 0)
	{
		help_exec(var, collect, path);
		execve(path, var->arr, env);
		perror("Minishell");
		reset_redirections(collect->fds);
		exit_free(collect, 127);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	    exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = WTERMSIG(status);
	if (path)
		free(path);
	return (exit_code);
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


int	execute(t_pipe *pipes, char ***env, t_free *collect)
{
	t_exe	*var;
	int fd[2];
	int status = -1;

	var = NULL;		 
	group_pipes(pipes, &var, collect, env);
	if (count_pipes(pipes) > 1)
	{
		int pipi = 	count_pipes(pipes);
		status = exec_pipe(var, env, pipi, collect);
	}
	else if (count_pipes(pipes) == 1)
	{
		dup_std(fd, collect);
		if (!is_builtin(var->arr[0]))
			status = exec_command(var, *env, collect);
		else
		{
			if (handle_redirections(var) < 0)
				return (1);
			if (var->arr)
				status = exec_builtin(var, env, collect);
		}
		reset_redirections(fd);
	}
	free_t_exe(&var);
	return status;
}
