/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:06 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/26 21:18:29 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *get_pwd(void)
{
	char *buff;
	
	buff = getcwd(NULL, 0);
	return buff;
}

int update_OLDPWD(char ***env, char *buff, t_free *collect)
{
	int i;

	i = 0;
	while ((*env)[i])
	{
		if ((ft_strncmp((*env)[i], "OLDPWD=", 7) == 0))
		{
			free((*env)[i]);
			(*env)[i] = ft_strjoin("OLDPWD=", buff);
			if (!(*env)[i])
				exit_free(collect, 1);
		}
		i++;
	}
	free(buff);
	return 1;
}

int update_PWD(char ***env, t_free *collect)
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
			free((*env)[i]);
			(*env)[i] = ft_strjoin("PWD=", str);
			if (!(*env)[i])
				return (free(str), exit_free(collect, 1), 1);
		}
	}
	free(str);
	return 1;
}

int	cd(char *path, char ***env, t_free *collect)
{	
	static char *buff = NULL;

	if (!buff)
		buff = get_pwd();
	if (chdir(path) != 0)
		return (putstr_fd("No Such a Directory\n", 2), 1);
	if (update_PWD(env, collect) == -1)
		return (putstr_fd("No Such a Directory\n", 2), 1);
	update_OLDPWD(env, buff, collect);
	buff = get_pwd();
	return (0);
}
