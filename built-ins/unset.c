/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:59 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/25 21:35:10 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	unset_no_val(char ***no_val, char *arg)
{
	char **clone;
	int i;
	int j;

	if (!arg)
		return;
	i = 0;
	j = 0;
	if (!(*no_val) || !(*no_val)[0])
		return ;
	clone = malloc((var_num(*no_val) + 1) * sizeof(char*));
	if (!clone)
		exit(1);
	while ((*no_val)[i])
	{
		if (ft_strcmp((*no_val)[i], arg) != 0)
			clone[j++] = ft_strdup((*no_val)[i]);
		i++;
	}
	clone[j] = NULL;
	free_2d_arr(*no_val);
	*no_val = clone;
}

int	unset(char ***env, char *arg, char ***no_val)
{
	char **clone;
	int i;
	int j;

	if (!arg)
		return 0;
	i = 0;
	j = 0;	
	unset_no_val(no_val, arg);
	clone = malloc((var_num(*env) + 1) * sizeof(char*));
	if (!clone)
		exit(1);
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], arg, ft_strlen(arg)) != 0)
			clone[j++] = ft_strdup((*env)[i]);
		i++;
	}
	clone[j] = NULL;
	free_2d_arr(*env);
	*env = clone;
	return 0;
}
