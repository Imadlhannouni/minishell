#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "minishell.h"

size_t var_num(char **arr)
{
	int i = 0;

	if (!arr)
		return 0;
	while (arr[i])
	{
		i++;
	}
	return i;
}

// void	free_arr(char **arr, int j)
// {
// 	while (j >= 0)
// 	{
// 		free(arr[j]);
// 		j--;
// 	}
// 	free(arr);
// }

char **clone_env(char **env)
{
	char **clone;
	int i = 0;

	clone = malloc((var_num(env) + 1) * sizeof(char*));
	if (!clone)
		return NULL;
	while (env[i])
	{
		clone[i] = ft_strdup(env[i]);
		if (!clone[i])
			return (free_arr(clone, i), NULL);
		i++;
	}
	clone[i] = NULL;
	return clone;
}