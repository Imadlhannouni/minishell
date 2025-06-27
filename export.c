#include "minishell.h"

int check_existence(char **env, char *name)
{
	int i ,len;

	i = 0;
	len = ft_strlen((const char*)name);
	while (env[i])
	{
		if (strncmp(env[i], name, len) == 0)
			return 1;
		i++;
	}
	return 0;
}

static char *join_strings(char *s1, char *s2, char *s3)
{
	char *temp;
	char *temp1;

	temp = ft_strjoin(s1, s2);
	temp1  = ft_strjoin(temp, s3);
	free(temp);
	return (temp1);
}

void replace_variable(char ***env, char **arg)
{
	int i = 0;

	while ((*env)[i])
	{
		if (strncmp((*env)[i], arg[0], ft_strlen(arg[0])) == 0)
		{
			free((*env)[i]);
			(*env)[i] = join_strings(arg[0], "=", arg[1]);
			return;
		}
		i++;
	}
}

char	**add_var(char **env, char **arg)
{
	char	**clone;
	int i = 0;

	clone = malloc((var_num(env) + 2) * sizeof(char*));
	if (!clone)
		return (NULL);
	while (env[i])
	{
		clone[i] = ft_strdup(env[i]);
		if (!clone[i])
			return (free_arr(clone, i), NULL);
		i++;
	}
	clone[i++] = join_strings(arg[0], "=", arg[1]);
	clone[i] = NULL;
	i = 0;
	while (env[i])
	{
		free(env[i++]);
	}
	free(env);
	return clone;
}

void	export(char ***env, char *full_arg)
{
	char **arg;

	if (full_arg == NULL) 
		return;
	arg = ft_split(full_arg, '=');
	if (check_existence(*env, arg[0]))
		replace_variable(env, arg);
	else
		*env = add_var(*env, arg);
	free(arg[0]);
	free(arg[1]);
	free(arg);
}
