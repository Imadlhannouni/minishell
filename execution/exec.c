/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:31 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/01 14:44:36 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void help_exec(t_exe *var, t_help *help)
{
	signal(SIGINT, help->prev_handler_int);
	signal(SIGQUIT, help->prev_handler_quit);
	if (handle_redirections(var) < 0)
	{
		close(help->std_fd[0]);
		close(help->std_fd[1]);		
		exit_free(127);
	}
}

int extract_status(int status)
{
	int exit_code;

	exit_code = -1;
	if (WIFEXITED(status))
	    exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		exit_code = WTERMSIG(status) + 128;
		if (exit_code == 130)
			write(1, "\n", 2);
		if (exit_code == 131)
			putstr_fd("Quit (core dumped)\n", 2);
	}
	return exit_code;
}

int	exec_command(t_exe *var, char **env, t_help *help)
{
	int		pid;
	int		status;
	char 	*path;

	status = -1;
	path = retrieve_path(var->arr[0], env);
	if (!path)
		return (putstr_fd("Minishell : Command not found\n",2), 127);
	pid = fork();
	if (pid < 0)
		return (putstr_fd("Minishell : fork() failed\n", 2), 1);
	if (pid == 0)
	{
		help_exec(var, help);
		execve(path, var->arr, env);
		perror("Minishell");
		reset_redirections(help->std_fd);
		exit_free(127);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sighandler);
	return (extract_status(status));
}

void reset_redirections(int fd[2])
{
	dup2(fd[0] ,STDIN_FILENO);
	dup2(fd[1] ,STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
}


void dup_std(int fd[2], t_help *help)
{
	help->std_fd = fd;
	fd[0] = dup(STDIN_FILENO);
	fd[1] =	dup(STDOUT_FILENO);
}


int	execute(t_pipe *pipes, char ***env, t_help *help)
{
	t_exe	*var;
	int fd[2];
	int status = -1;

	var = NULL;	 
	group_pipes(pipes, &var);
	if (count_pipes(pipes) > 1)
		status = exec_pipe(var, env, count_pipes(pipes), help);
	else if (count_pipes(pipes) == 1)
	{
		dup_std(fd, help);
		if (!is_builtin(var->arr[0]))
			status = exec_command(var, *env, help);
		else
		{
			if (handle_redirections(var) < 0)
				return (1);
			if (var->arr)
				status = exec_builtin(var, env);
		}
		reset_redirections(fd);
	}
	return status;
}
