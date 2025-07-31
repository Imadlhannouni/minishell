/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:46 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/31 21:58:13 by abbenmou         ###   ########.fr       */
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
			path = strdup(env[i] + 5);
			paths = ft_split(path, ':');
			free(path);
			break;
		}
		i++; 
	}
	return (paths);
}
char	*retrieve_path(char *cmd, char **env)
{
	int i;
	char *path;
	char **paths;

	paths = get_PATH(env);
	if (!*cmd)
		return (free_2d_arr(paths), NULL);
	if (is_path1(cmd) || !paths || !*paths)
		return (ft_strdup(cmd));
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin_v2(paths[i], "/", 0);
		path = ft_strjoin_v2(path, cmd, 1);
		if (access(path,F_OK | X_OK) == 0)
			return (path);
		i++;
	}
	return (NULL);
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

void close_previous(int (*fd)[2], int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

int count_pipes(t_pipe *pipes)
{
	t_pipe *temp;
	int cpt;

	temp = pipes;
	cpt = 0;
	while (temp)
	{
		cpt++;
		temp = temp->nextpipe;
	}
	return cpt;
}