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

int	count_args(t_token *tok)
{
	t_token *temp;
	int cpt;

	temp = tok;
	cpt = 0;
	while (temp)
	{
		cpt++;
		temp = temp->next;
	}
	return cpt;
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

char	**group_2d_arr(t_token *tok)
{
	t_token	*temp;
	char **arr;
	int i;

	i = 0;
	temp = tok;
	arr = malloc((count_args(tok) + 1) * sizeof(char*));
	if (!arr)
		return NULL;
	while (temp)
	{
		arr[i++] = ft_strdup(temp->value);
		if (!arr[i - 1])
			return (free_arr(arr, i - 1),NULL);//need to free the arr
		temp = temp->next;
	}
	arr[i] = NULL;
	return arr;
}

char	***group_3d_arr(t_pipe *pipes)
{
	char ***global;
	t_pipe *temp;
	int i;

	i = 0;
	temp = pipes;
	global = malloc((count_pipes(pipes) + 1) * sizeof(char**));
	if (!global)
		return NULL;
	while (temp)
	{
		global[i++] = group_2d_arr(temp->full_cmd);
		if (!global[i - 1])
			return NULL;//need to free arr and global
		temp = temp->nextpipe;
	}
	global[i] = NULL;
	return global;
}
void	print_3d_arr(char ***arr)
{
	int i = 0;
	int j = 0;


	while (arr[i])
	{
		while(arr[i][j])
		{
			printf("%s    ",arr[i][j]);
			j++;
		}
		printf("||   ");
		i++;
		j=0;
	}
	printf("\n");
}
void exec_command(char **arg, char **env)
{
	int pid;
	pid = fork();
	if (pid == 0)
	{
		execve(retrieve_path(arg[0],env), arg, env);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
}

void	execute(t_pipe *pipes, char **env)
{
	int count = count_pipes(pipes);

	if (count > 1)
	{
		char ***args=group_3d_arr(pipes);
		exec_pipe(args, env);
	}
	else if (count == 1)
	{
		char **arg = group_2d_arr(pipes->full_cmd);
		exec_command(arg, env);
		free_2d_arr(arg);
	}
}