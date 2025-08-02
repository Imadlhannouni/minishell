/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:02 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/02 15:29:40 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "../minishell.h"


int	pwd(char **env)
{	
	char	*buffer;
	int		i;
	
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PWD=", 4) == 0)
		{
			putstr_fd((env[i] + 4), 1);
			return (putstr_fd("\n", 1), 0);
		}
		i++;
	}
	buffer = getcwd(NULL,0);
	if (!buffer)
	{
		putstr_fd("error retrieving current directory\n", 2);
		return (1);
	}
	putstr_fd(buffer, 1);
	write(1, "\n", 1);
	if (buffer)
		free(buffer);
	return (0);
}
