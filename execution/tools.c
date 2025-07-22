#include "../minishell.h"


int is_builtin(char *cmd)
{
	if (cmd == NULL)
		return 0;
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

void exec_builtin(t_exe *var, char ***env, char ***no_val)
{
	if (strcmp(var->arr[0], "cd") == 0)
		cd(var->arr[1], env);
	else if (strcmp(var->arr[0], "pwd") == 0)
		pwd();
	else if (strcmp(var->arr[0], "echo") == 0)
		echo(var->arr);
	else if (strcmp(var->arr[0], "export") == 0)
		export(env, &(var->arr[1]), no_val) ;
	else if (strcmp(var->arr[0], "unset") == 0)
		unset(env, var->arr[1]);
	else if (strcmp(var->arr[0], "exit") == 0)
		exit(0);
	else if (strcmp(var->arr[0], "env") == 0)
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