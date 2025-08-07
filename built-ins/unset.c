/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:59 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/07 14:19:59 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check(char *env, char **arg)
{
	int		i;
	char	**tmp;

	i = 0;
	tmp = ft_split(env, '=');
	while (arg[i])
	{
		if (ft_strcmp(arg[i], tmp[0]) == 0)
			return (1);
		i++;
	}
	return (0);
}

static size_t	count_to_remove(char **arr, char **arg)
{
	int	i;
	int	cpt;

	i = 0;
	cpt = 0;
	if (!arr || !arg)
		return (0);
	while (arr[i])
	{
		if (check(arr[i], arg))
			cpt++;
		i++;
	}
	return (cpt);
}

int	check_equ(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

static int	check_exe(char **env, char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (check_existence(env, arg[i]))
			return (1);
		i++;
	}
	return (0);
}

int	unset(char ***env, char **arg)
{
	char	**clone;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!arg || !check_exe(*env, arg))
		return (0);
	clone = ft_malloc((var_num(*env)
				- count_to_remove(*env, arg) + 1) * sizeof(char*), 0);
	while ((*env)[i])
	{
		if (!check((*env)[i], arg))
			clone[j++] = ft_strdup((*env)[i]);
		i++;
	}
	clone[j] = NULL;
	*env = clone;
	return (0);
}
