/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:05 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/08 16:16:48 by abbenmou         ###   ########.fr       */
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
			printf("%s\n", env[i]);
		}
		i++;
	}
	return (0);
}
