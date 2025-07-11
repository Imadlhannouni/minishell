#include "../minishell.h"

char	*retrieve_path(char *cmd, char **env)
{
	int i = 0;
	char *path;
	char **paths;

	paths = NULL;
	while (env[i])
	{
		if (strncmp(env[i],"PATH=",5) == 0)
		{
			path = strdup(env[i] + 5);
			paths = ft_split(path, ':');
			free(path);
			break;
		}
		i++; 
	}
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin_v2(paths[i], "/", 0);
		path = ft_strjoin_v2(path, cmd, 1);
		if (access(path,F_OK | X_OK) == 0)
			return (free_2d_arr(paths),path);
		else
			free(path);
		i++;
	}
	free_2d_arr(paths);
	return (NULL);
}

void	free_2d_arr(char **arr)
{
	int i = 0;

	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_all(char ***arr)
{
	int i = 0;
	int j = 0;

	if (!arr)
		return;
	while (arr[i])
	{
		while (arr[i][j])
		{
			free(arr[i][j]);
			j++;
		}
		free(arr[i]);
		i++;
		j = 0;
	}
	free(arr);
}
