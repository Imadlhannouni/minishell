/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:05 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/07 14:13:50 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_env(char **env, char **args)
{
	int	i;

	if (!env || args[1])
		return (1);
	i = 0;
	while (env[i])
	{
		if (!check_equ(env[i]))
		{
			putstr_fd(env[i], 1);
			putstr_fd("\n", 1);
		}
		i++;
	}
	return (0);
}
