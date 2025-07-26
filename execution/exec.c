/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:31 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/25 21:54:40 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_2d_arr(char **arr)
{
	int i = 0;
	while (arr[i])
	{
		printf("arr[%d] == %s\n",i,arr[i]);
		i++;
	}
}
void	print_red(t_red *red)
{
	t_red *tmp;
	tmp = red;
	while (tmp)
	{
		printf("|| red_type == %d || red_file = %s ||\n",tmp->red_type,tmp->file);
		tmp = tmp->next;
	}
}

int exec_command(t_exe *var, char **env)
{
	int pid;
	int status;
	char *path;

	if (!is_path1(var->arr[0]))
		path = retrieve_path(var->arr[0],env);
	else
		path = ft_strdup(var->arr[0]);
	if (!path)
		return (putstr_fd("Command not found\n",2), 127);
	pid = fork();
	if (pid == 0)
	{
		if (handle_redirections(var) < 0)
			exit(1);
		execve(path, var->arr, env);
		exit(127);
	}
	else
		waitpid(pid, &status, 0);
	free(path);
	return (status);
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

void reset_redirections(int fd[2])
{
	dup2(fd[0] ,STDIN_FILENO);
	dup2(fd[1] ,STDOUT_FILENO);
}

int	execute(t_pipe *pipes, char ***env)
{
	t_exe	*var;
	int fd[2];
	int status = -1;

	fd[0] = dup(STDIN_FILENO);
	fd[1] =	dup(STDOUT_FILENO);
	var = NULL;
	group_pipes(pipes, &var);
	if (count_pipes(pipes) > 1)
		status = exec_pipe(var, env, count_pipes(pipes));
	else if (count_pipes(pipes) == 1)
	{
		if (!is_builtin(var->arr[0]))
			status = exec_command(var, *env);
		else
		{
			if (handle_redirections(var) < 0)
				return (1);
			if (var->arr)
				status = exec_builtin(var, env);
		}
		reset_redirections(fd);
	}
	free_t_exe(&var);
	return status;
}
