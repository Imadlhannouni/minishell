/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:06 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/25 21:26:58 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int update_PWD(char ***env)
{
	int i;
	char *str;

	i = -1;
	while ((*env)[++i])
	{
		if (ft_strncmp((*env)[i], "PWD=", 4) == 0)
		{
			str = get_pwd();
			if (!str)
			{
				putstr_fd("Parent Directory Has Been Deleted\n", 2);
				if (chdir("/home"))
					return (putstr_fd("/home Does Not Exist\n", 2), -1);
				str = get_pwd();
			}
			(*env)[i] = ft_strjoin("PWD=",get_pwd());
		}
	}
	free(str);
	return 1;
}

int	cd(char *path, char ***env)
{	
	static char *buff = NULL;

	if (!buff)
		buff = get_pwd();
	if (chdir(path) != 0)
		return (putstr_fd("No Such a Directory\n", 2), 1);
	if (update_PWD(env) == -1)
		return (putstr_fd("No Such a Directory\n", 2), 1);
	update_OLDPWD(env, buff);
	buff = get_pwd();
	return (0);
}


  