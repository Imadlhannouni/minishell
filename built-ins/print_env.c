/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:05 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/31 20:21:16 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int print_env(char **env, char **args)
{
	if(!env || args[1])
		return 1;
	int i = 0;
	while (env[i])
	{
		if (!check_equ(env[i]))
		{
			putstr_fd(env[i], 1);
			write(1 ,"\n", 1);
		}
		i++;
	}
	return 0;
}