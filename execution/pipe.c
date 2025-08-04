/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:46 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/04 15:15:56 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char **get_PATH(char **env)
{
	char	*path;
	char	**paths;
	int	i;

	i = 0;
	path = NULL;
	paths = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i],"PATH=",5) == 0)
		{
			path = ft_strdup(env[i] + 5);
			paths = ft_split(path, ':');
			break;
		}
		i++; 
	}
	return (paths);
}

static int check_path1(char *cmd, char **path)
{
	if (is_path1(cmd) && access(cmd, F_OK) == 0)
	{
		if (check_directory(cmd))
			return (*path = cmd, 1);
		if (access(cmd, X_OK) == 0)
			return (*path = cmd, 0);
		else
			return (*path = cmd, 126);
	}
	else if (is_path1(cmd))
		return (*path = cmd, 2);
	return 0;
}

int	retrieve_path(char *cmd, char **env, char **path)
{
	char *tmp;
	char **paths;
	
	if (!*cmd)
		return (*path = NULL, 127);
	if (is_path1(cmd))
		return (check_path1(cmd, path));
	paths = get_PATH(env);
	if (!paths || !*paths)
		return (*path = ft_strdup(cmd), 0);
	while (*paths)
	{
		tmp = join_strings(*paths, "/", cmd);
		if (access(tmp, F_OK) == 0)
		{
			if (access(tmp, X_OK) == 0)
				return (*path = tmp, 0);
			else if (!*path)
				*path = tmp;
		}
		paths++;
	}
	if (*path)
		return (126);
	return (127);
}

void close_fd(int (*fd)[2], size_t i, size_t total) 
{
	size_t	j;

	j = 0;
	if (i == 0)
	{
		close(fd[i][0]);
		j++;
	}
	else if (i == total - 1)
	{
		close(fd[i - 1][1]);
	}
	while (j < total - 1)
	{
		if (j == i - 1)
			close(fd[j++][1]);
		else if (j == i)
			close(fd[j++][0]);
		else if (j != i - 1 && j != i)
		{
			close(fd[j][0]);
			close(fd[j++][1]);
		}
	}
}

void	switch_fd(int (*fd)[2], size_t i, size_t total)
{
	if (i == 0)
	{
		dup2(fd[0][1], STDOUT_FILENO);
		close(fd[0][1]);
	}
	else if (i == total)
	{
		dup2(fd[i - 1][0], STDIN_FILENO);
		close(fd[i - 1][0]);
	}
	else if (i < total)
	{
		dup2(fd[i - 1][0], STDIN_FILENO);
		dup2(fd[i][1], STDOUT_FILENO);
		close(fd[i - 1][0]);
		close(fd[i][1]);
	}
}