#include <dirent.h>
#include "../minishell.h"

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
			return (free_arr(arr, i - 1),NULL);
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
			return (free_3d_arr(global, i - 2),NULL);
		temp = temp->nextpipe;
	}
	global[i] = NULL;
	return global;
}

void exec_command(char **arg, char **env)
{
	int pid;
	char *path = retrieve_path(arg[0],env);
	if (!path)
	 	return;
	pid = fork();
	if (pid == 0)
	{
		execve(path, arg, env);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
	free(path);
}
int is_builtin(char *cmd)
{
	if (strcmp(cmd, "cd") == 0)
		return 1;
	else if (strcmp(cmd, "pwd") == 0)
		return 1;
	else if (strcmp(cmd, "echo") == 0)
		return 1;
	else if (strcmp(cmd, "export") == 0)
		return 1;
	else if (strcmp(cmd, "unset") == 0)
		return 1;
	else if (strcmp(cmd, "exit") == 0)
		return 1;
	else if (strcmp(cmd, "env") == 0)
		return 1;
	return 0;
}

void exec_builtin(char **arg, char ***env, char ***no_val)
{
	if (strcmp(arg[0], "cd") == 0)
		cd(arg[1], env);
	else if (strcmp(arg[0], "pwd") == 0)
		pwd();
	else if (strcmp(arg[0], "echo") == 0)
		echo(arg);
	else if (strcmp(arg[0], "export") == 0)
		export(env, &arg[1], no_val) ;
	else if (strcmp(arg[0], "unset") == 0)
		unset(env, arg[1]);
	else if (strcmp(arg[0], "exit") == 0)
		exit(0);
	else if (strcmp(arg[0], "env") == 0)
		print_env(*env);
}

void	execute(t_pipe *pipes, char ***env)
{
	int count = count_pipes(pipes);
	static char **no_val = NULL;

	if (count > 1)
	{
		char ***args=group_3d_arr(pipes);
		exec_pipe(args, env, &no_val);
	}
	else if (count == 1)
	{
		char **arg = group_2d_arr(pipes->full_cmd);
		if (!is_builtin(arg[0]))
			exec_command(arg, *env);
		else
			exec_builtin(arg,env, &no_val);
		free_2d_arr(arg);
	}
}
