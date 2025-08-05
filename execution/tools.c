/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:26 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/05 18:33:19 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_builtin(char *cmd)
{
	if (!cmd)
		return -1;
	if (ft_strcmp(cmd, "cd") == 0)
		return 1;
	else if (ft_strcmp(cmd, "pwd") == 0)
		return 1;
	else if (ft_strcmp(cmd, "echo") == 0)
		return 1;
	else if (ft_strcmp(cmd, "export") == 0)
		return 1;
	else if (ft_strcmp(cmd, "unset") == 0)
		return 1;
	else if (ft_strcmp(cmd, "exit") == 0)
		return 1;
	else if (ft_strcmp(cmd, "env") == 0)
		return 1;
	return 0;
}

int exec_builtin(t_exe *var, char ***env, t_help *help)
{
	if (!var->arr || !var->arr[0])
		return 1;
	if (var->is_ambiguis)
	{
		putstr_fd("Minishell : ambiguous redirect\n", 2);
		return (1);
	}
	if (ft_strcmp(var->arr[0], "cd") == 0)
		return cd(var->arr, env);
	else if (ft_strcmp(var->arr[0], "pwd") == 0)
		return pwd(*env);
	else if (ft_strcmp(var->arr[0], "echo") == 0)
		return (echo(var->arr));
	else if (ft_strcmp(var->arr[0], "export") == 0)
		return export(env, &(var->arr[1])) ;
	else if (ft_strcmp(var->arr[0], "unset") == 0)
		return unset(env, &(var->arr[1]));
	else if (ft_strcmp(var->arr[0], "exit") == 0)
		return exit_shell(var, help);
	else if (ft_strcmp(var->arr[0], "env") == 0)
		return print_env(*env, var->arr);
	return 0;
}

size_t var_num(char **arr)
{
	int i = 0;

	if (!arr)
		return 0;
	while (arr[i])
	{
		i++;
	}
	return i;
}

char **clone_env(char **env)
{
	char **clone;
	int i = 0;

	clone = (char**)ft_malloc((var_num(env) + 1) * sizeof(char*), 0);
	while (env[i])
	{
		clone[i] = ft_strdup(env[i]);
		i++;
	}
	clone[i] = NULL;
	return (clone);
}


int reset_redirections(int fd[2])
{
	if (dup2(fd[0] ,STDIN_FILENO) < 0)
		return (perror("Minishell"),-1);
	if (dup2(fd[1] ,STDOUT_FILENO) < 0)
		return (perror("Minishell"),-1);
	close(fd[0]);
	close(fd[1]);
	return 1;
}