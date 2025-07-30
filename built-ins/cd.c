/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:06 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/30 11:56:19 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *get_pwd(void)
{
	char *buff;
	
	buff = getcwd(NULL, 0);
	return buff;
}

int update_OLDPWD(char ***env, t_free *collect)
{
	int i;
	char *oldpwd;

	i = -1;
	oldpwd = NULL;
	while ((*env)[++i])
	{
		if (ft_strncmp((*env)[i], "PWD=", 4) == 0)
		{
			oldpwd = ft_strdup((*env)[i] + 4);
			if (!oldpwd)
				exit_free(collect, 1);
		}
	}
	if (!oldpwd)
		return -1;
	i = -1;
	while ((*env)[++i])
	{
		if ((ft_strncmp((*env)[i], "OLDPWD=", 7) == 0))
		{
			free((*env)[i]);
			(*env)[i] = ft_strjoin("OLDPWD=", oldpwd);
			if (!(*env)[i])
				exit_free(collect, 1);
		}
	}
	free(oldpwd);
	return 1;
}

int update_PWD(char ***env, t_free *collect)
{
	int i;
	char *str;

	str = NULL;
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
	if (str)
		free(str);
	return 1;
}

int	cd(char **arr, char ***env, t_free *collect)
{	
	if (chdir(arr[1]) != 0)
		return (putstr_fd("No Such a Directory\n", 2), 1);
	if (arr[2])
	{
		putstr_fd("cd : too many arguments\n", 2);
		return (1);
	}
	update_OLDPWD(env, collect);
	if (update_PWD(env, collect) == -1)
		return (1);
	return (0);
}
