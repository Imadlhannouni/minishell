#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "minishell.h"


char *retreive_var(char **env, char *var)
{
	int i = 0;
	while (env[i])
	{
		if (strncmp(env[i], var, ft_strlen(var)) == 0)
			return (ft_strdup(env[i] + ft_strlen(var)));
		i++;
	}
	return NULL;
}

static char *get_pwd(void)
{
	char *buff;
	int i;

	i = 0;
	buff = malloc(200 * sizeof(char));
	while (i < 200)
	{
		buff[i++] = 0;
	}
	i = 0;
	getcwd(buff, 200);
	return buff;
}

void	cd(char *path, char ***env)
{	
	int i = 0;
	char *buff;

	buff = get_pwd();
	if (chdir(path) != 0)
		perror("no such a directory");	
	else
	{
		while ((*env)[i])
		{
			if (strncmp((*env)[i], "PWD=", 4) == 0)
			{
				free((*env)[i]);
				(*env)[i] = ft_strjoin("PWD=", get_pwd());
			}
			else if ((strncmp((*env)[i], "OLDPWD=", 7) == 0))
			{
				free((*env)[i]);
				(*env)[i] = ft_strjoin("OLDPWD=", buff);
				free(buff);
			}
			i++;
		}
	}
}


  