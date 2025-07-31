/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:59 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/31 12:00:31 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int check(char *env, char **arg)
{
	int i;
	
	i = 0;
	while (arg[i])
	{
		if (ft_strncmp(arg[i], env, ft_strlen(arg[i])) == 0)
			return 1;
		i++;
	}
	return 0;
}

static size_t count_to_remove(char **arr, char **arg)
{
	int i;
	int cpt;

	i = 0;
	cpt = 0;
	if (!arr || !arg)
		return 0;
	while (arr[i])
	{
		if (check(arr[i], arg))
			cpt++;
		i++;			
	}
	return cpt;
}

// static void help_unset()
// {
	
// }

static void	unset_no_val(char ***no_val, char **arg, t_free *collect)
{
	char **clone;
	int i;
	int len;
	int j;

	if (!arg)
		return;
	i = 0;
	j = 0;
	len = var_num(*no_val) - count_to_remove(*no_val, arg);
	if (len == 0)
	{
		free_2d_arr(*no_val);
		*no_val = NULL;
		return ;
	}	
	if (!(*no_val))
		return ;
	clone = malloc((len + 1) * sizeof(char*));
	if (!clone)
		exit_free(collect, 1);
	while ((*no_val)[i])
	{
		if (!check((*no_val)[i], arg))
		{
			clone[j++] = ft_strdup((*no_val)[i++]);
			if (!clone[j - 1])
				return (free_arr(clone, j - 1), exit_free(collect, 1));
		}
		else
			i++;
	}
	clone[j] = NULL;
	free_2d_arr(*no_val);
	*no_val = clone;
}

static int check_exe(char **env, char **arg)
{
	int i;

	i = 0;
	while (arg[i])
	{
		if (check_existence(env, arg[i]))
			return 1;
		i++;
	}
	return 0;
}

int	unset(char ***env, char **arg, char ***no_val, t_free *collect)
{
	char **clone;
	int i;
	int j;

	i = 0;
	j = 0;
	if (!arg || (!check_exe(*env, arg) && !check_exe(*no_val, arg)))
		return 0;
	unset_no_val(no_val, arg, collect);
	clone = malloc((var_num(*env) - count_to_remove(*env, arg) + 1) * sizeof(char*));
	if (!clone)
		exit_free(collect, 1);
	while ((*env)[i])
	{
		if (!check((*env)[i], arg))
			clone[j++] = ft_strdup((*env)[i]);
		i++;
	}
	clone[j] = NULL;
	free_2d_arr(*env);
	*env = clone;
	return 0;
}
