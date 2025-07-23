/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:59 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/23 22:51:00 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unset(char ***env, char *arg)
{
	if (!arg)
		return;
	char **clone = malloc(var_num(*env) * sizeof(char*));
	int i = 0;
	int j = 0;

	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], arg, ft_strlen(arg)) != 0)
			clone[j++] = (*env)[i];
		i++;
	}
	clone[j] = NULL;
	*env = clone;
}