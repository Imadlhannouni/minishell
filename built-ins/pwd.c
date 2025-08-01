/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:02 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/01 09:57:05 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "../minishell.h"


int	pwd(void)
{	
	char *buffer;
	
	buffer = getcwd(NULL,0);
	if (!buffer)
	{
		putstr_fd("Minishell : PWD failed\n", 2);
		return (1);
	}
	putstr_fd(buffer, 1);
	write(1, "\n", 1);
	free(buffer);
	return (0);
}