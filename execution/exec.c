/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:31 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/23 22:50:32 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void exec_command(t_exe *var, char **env)
{
	int pid;
	char *path = retrieve_path(var->arr[0],env);
	if (!path)
	 	return;
	pid = fork();
	if (pid == 0)
	{
		handle_redirections(var, NULL);
		execve(path, var->arr, env);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
	free(path);
}

void group_pipes(t_pipe *pipes, t_exe **var)
{
	t_pipe *tmp;

	if (!pipes)
		return;
	tmp = pipes;
	while (tmp)
	{
		add_node(var,creat_node(tmp->full_cmd));
		tmp = tmp->nextpipe;
	}
}

void reset_redirections(t_exe *var,int fd[2])
{
	if (fd[0] >= 0 || fd[1] >= 0)
	{
		if (var->out_red_type)
			dup2(fd[1] ,STDOUT_FILENO);
		if (var->in_red_type)
			dup2(fd[0] ,STDIN_FILENO);
	}
}

void	execute(t_pipe *pipes, char ***env)
{
	t_exe	*var = NULL;
	static char **no_val = NULL;
	int fd[2];

	fd[0] = -1;
	fd[1] = -1;
	group_pipes(pipes, &var);
	if (count_pipes(pipes) > 1)
		exec_pipe(var, env, &no_val, count_pipes(pipes));
	else if (count_pipes(pipes) == 1)
	{
		if (is_builtin(var->arr[0]) == 0)
			exec_command(var, *env);
		handle_redirections(var, fd);
		if (var->arr)
			exec_builtin(var, env, &no_val);
		reset_redirections(var, fd);
	}
	free_t_exe(&var);
}
