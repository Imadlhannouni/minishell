#include "../minishell.h"


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
	return (clone);
}