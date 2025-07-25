/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:02 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/23 22:51:03 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "../minishell.h"


void	pwd(void)
{	
	char *buffer;
	
	buffer = getcwd(NULL,0);
	printf("%s\n",buffer);
}