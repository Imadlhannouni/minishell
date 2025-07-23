/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:06 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/23 22:50:07 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "../minishell.h"

static char *get_pwd(void)
{
	char *buff;
	
	buff = getcwd(NULL, 0);
	return buff;
}

int update_OLDPWD(char ***env, char *buff)
{
	int i;

	i = 0;
	while ((*env)[i])
	{
		if ((ft_strncmp((*env)[i], "OLDPWD=", 7) == 0))
		{
			free((*env)[i]);
			(*env)[i] = ft_strjoin("OLDPWD=", buff);
		}
		i++;
	}
	return 1;
}

int update_PWD(char ***env, char *buff)
{
	int i;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], "PWD=", 4) == 0)
			{
				char *str = get_pwd();
				if (!str)
				{
					chdir("/home");
					free((*env)[i]);
					(*env)[i] = ft_strjoin("PWD=",get_pwd());
					update_OLDPWD(env, buff);
					perror("parent directory has been deleted\n");
					return 0;
				}
				free((*env)[i]);
				(*env)[i] = ft_strjoin("PWD=",str);
			}
		i++;
	}
	return 1;
}

void	cd(char *path, char ***env)
{	
	static char *buff = NULL;

	if (!buff)
		buff = get_pwd();
	if (chdir(path) != 0)
		perror("no such a directory");
	if (!update_PWD(env, buff))
		return;
	update_OLDPWD(env, buff);
	buff = get_pwd();
}


  