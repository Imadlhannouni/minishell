/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:31 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/25 12:39:32 by abbenmou         ###   ########.fr       */
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

void exec_command(t_exe *var, char **env)
{
	int pid;
	char *path;

	if (!is_path1(var->arr[0]))
		path = retrieve_path(var->arr[0],env);
	else
		path = ft_strdup(var->arr[0]);
	if (!path)
	 	return;
	// print_2d_arr(var->arr);
	// print_red(var->redirections);	
	pid = fork();
	if (pid == 0)
	{
		handle_redirections(var);
		if (execve(path, var->arr, env) == -1)
		{
			put_str_fd("command not fount", 2);
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, NULL, 0);
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

void reset_redirections(int fd[2])
{
	dup2(fd[0] ,STDIN_FILENO);
	dup2(fd[1] ,STDOUT_FILENO);
}

void	execute(t_pipe *pipes, char ***env)
{
	t_exe	*var = NULL;
	static char **no_val = NULL;
	int fd[2];

	fd[0] = dup(STDIN_FILENO);
	fd[1] =	dup(STDOUT_FILENO);
	group_pipes(pipes, &var);
	if (count_pipes(pipes) > 1)
		exec_pipe(var, env, &no_val, count_pipes(pipes));
	else if (count_pipes(pipes) == 1)
	{
		if (!is_builtin(var->arr[0]))
			exec_command(var, *env);
		else
		{
			handle_redirections(var);
			if (var->arr)
				exec_builtin(var, env, &no_val);
		}
		reset_redirections(fd);
	}
	free_t_exe(&var);
}
