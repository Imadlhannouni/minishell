#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "../minishell.h"


char *retreive_var(char **env, char *var)
{
	int i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, ft_strlen(var)) == 0)
			return (ft_strdup(env[i] + ft_strlen(var)));
		i++;
	}
	return NULL;
}

static char *get_pwd(void)
{
	char *buff;
	
	buff = getcwd(NULL, 0);
	return buff;
}

void	cd(char *path, char ***env)
{	
	int i = 0;
	static char *buff = NULL;

	if (!buff)
		buff = get_pwd();
	if (chdir(path) != 0)
		perror("no such a directory");	
	else
	{
		while ((*env)[i])
		{
			if (ft_strncmp((*env)[i], "PWD=", 4) == 0)
			{
				free((*env)[i]);
				(*env)[i] = ft_strjoin("PWD=", get_pwd());
			}
			if ((ft_strncmp((*env)[i], "OLDPWD=", 7) == 0))
			{
				free((*env)[i]);
				(*env)[i] = ft_strjoin("OLDPWD=", buff);
				free(buff);
			}
			i++;
		}
	}
	buff = get_pwd();
}


  