/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:26 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/27 17:00:35 by abbenmou         ###   ########.fr       */
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

int exec_builtin(t_exe *var, char ***env, t_free *collect)
{
	static char **no_val = NULL;

	if (!var->arr || !var->arr[0])
		return 1;
	no_val = NULL;
	if (ft_strcmp(var->arr[0], "cd") == 0)
		return cd(var->arr, env, collect);
	else if (ft_strcmp(var->arr[0], "pwd") == 0)
		return pwd();
	else if (ft_strcmp(var->arr[0], "echo") == 0)
		return (echo(var->arr));
	else if (ft_strcmp(var->arr[0], "export") == 0)
		return export(env, &(var->arr[1]), &no_val) ;
	else if (ft_strcmp(var->arr[0], "unset") == 0)
		return unset(env, var->arr[1], &no_val);
	else if (ft_strcmp(var->arr[0], "exit") == 0)
		exit_shell(var, collect);
	else if (ft_strcmp(var->arr[0], "env") == 0)
		return print_env(*env, var->arr);
	return 0;
}

void	free_2d_arr(char **arr)
{
	int i = 0;

	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
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

	clone = malloc((var_num(env) + 1) * sizeof(char*));
	if (!clone)
		return NULL;
	while (env[i])
	{
		clone[i] = ft_strdup(env[i]);
		if (!clone[i])
			return (free_arr(clone, i), NULL);
		i++;
	}
	clone[i] = NULL;
	return (clone);
}