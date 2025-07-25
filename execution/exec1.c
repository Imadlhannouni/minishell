/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:21 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/25 16:24:34 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_args(t_token *tok)
{
	t_token *temp;
	int cpt;

	if (!tok)
		return 0;
	temp = tok;
	cpt = 0;

	while (temp != NULL)
	{
		if ((temp->inp_red == 0) && (temp->heredoc == 0)
			&& (temp->out_red == 0) && (temp->out_app == 0))
			cpt++;
		temp = temp->next;
	}
	return cpt;
}

int	init_var(t_vars *var, size_t pipe_num)
{
	size_t j;

	j = 0;
	var->i = 0;
	var->pipe_num = pipe_num;
	if (pipe_num > 1)
	{
		var->pid = malloc((pipe_num) * sizeof(__pid_t));
		if (!var->pid)
			exit(2);
		var->fd = malloc((pipe_num - 1) * sizeof(int[2]));
		if (!var->fd)
			return (free(var->pid), exit(2), 1);
		while (j < var->pipe_num - 1)
		{
			if (pipe((var->fd)[j]) == -1)
				return (close_previous(var->fd, j - 1),free(var->fd),free(var->pid),1);
			j++;
		}
	}
	return 1;
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

int helper(t_exe *tmp ,char ***env, char ***no_val, t_vars var)
{
	char *path = NULL;

	if (!is_builtin(tmp->arr[0]) && !is_path1(tmp->arr[0]))
		path = retrieve_path(tmp->arr[0],*env);
	else if (is_path1(tmp->arr[0]))
		path = ft_strdup(tmp->arr[0]);
	int pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		close_fd(var.fd, var.i, var.pipe_num);
		switch_fd(var.fd, var.i, var.pipe_num - 1);
		if (handle_redirections(tmp) < 0)
			exit(1);
		if (!is_builtin(tmp->arr[0]))
		{
			execve(path, tmp->arr, *env);
			exit(127);
		}
		return exec_builtin(tmp, env, no_val);
		exit(0);
	}
	free(path);
	return pid;
}

int exec_pipe(t_exe *grp, char ***envp, char ***no_val, size_t pipe_num)
{
	t_vars var;
	t_exe *tmp;

	if (!init_var(&var, pipe_num))
		return 1;
	tmp = grp;
	while (var.i < pipe_num) 
	{
		var.pid[var.i++] = helper(tmp, envp, no_val, var);
		tmp = tmp->next;
	}
	close_previous(var.fd, var.pipe_num - 1);
	var.i = 0;
	waitpid(var.pid[var.pipe_num - 1],NULL,0);
	while (var.i++ < var.pipe_num - 1)
		wait(NULL);
	free(var.pid);
	free(var.fd);
	return 0;
}


