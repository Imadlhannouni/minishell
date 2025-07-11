#include "../minishell.h"

int check_existence(char **env, char *name)
{
	int i ,len;

	i = 0;
	len = ft_strlen((const char*)name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0)
			return 1;
		i++;
	}
	return 0;
}

char *join_strings(char *s1, char *s2, char *s3)
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
		if (ft_strncmp((*env)[i], arg[0], ft_strlen(arg[0])) == 0)
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

static char *fill_word(int start, int end, char *str)
{
	char *s;
	int i = 0;
	s = malloc((end - start + 1) * sizeof(char));
	if (!s)
		return NULL;
	while (i < end - start)
	{
		s[i] = str[start + i];
		i++;
	}
	s[i] = '\0';
	return s;
}
int check(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return 1;
		i++;
	}
	return 0;
}

char **spec_split(char *str)
{
	char **arr = NULL;
	int i = 0;

	if (!str)
		return NULL;
	if (!check(str))
	{
		arr = malloc(2 * sizeof(char *));
		if (!arr)
			return NULL;
		arr[0] = ft_strdup(str);
		arr[1] = NULL;
		return arr;
	}
	arr = malloc(3 * sizeof(char*));
	if (!arr)
		return NULL;
	while (str[i] != '=')
		i++;
	arr[0] = fill_word(0, i, str);
	if (!arr[0])
		return (NULL);
	arr[1] = fill_word(++i, ft_strlen(str), str);
	if (!arr[1])
		return (free(arr[0]), NULL);
	arr[2] = NULL;
	return arr;
}

void store_no_val(char ***args, char ***env, char *arg)
{
	int i = 0;
	if (*args == NULL)
	{
		*args = malloc(2 * sizeof(char*));
		if (!*args)
			return;
		(*args)[0] = ft_strdup(arg);
		(*args)[1] = NULL;
	}
	else if (check_existence(*env, arg) == 1)
		return;
	else
	{
		char **new;
		new = malloc((var_num(*args) + 2) * sizeof(char *));
		if (!new)
			return;
		while ((*args)[i])
		{
			new[i] = ft_strdup((*args)[i]);
			i++;
		}
		new[i++] = ft_strdup(arg);
		new[i] = NULL;
		i = 0;
		while ((*args)[i])
		{
			free((*args)[i]);
			i++;
		}
		free(*args);
		*args = new; 
	}
	i = 0;
}

void	export(char ***env, char *full_arg, char ***no_val)
{
	char **arg = NULL;
	char **args = NULL;
	int i = 0;

	if (full_arg == NULL)
	{
		print_sorted(*env, *no_val);
		return;
	}
	args = ft_split(full_arg, ' ');
	while (args[i])
	{
		if (check(args[i]) == 0)
			store_no_val(no_val, env, args[i]);
		else
		{
			arg = spec_split(args[i]);
			if (check_existence(*env, arg[0]))
				replace_variable(env, arg);
			else
				*env = add_var(*env, arg);
			free_2d_arr(arg);
		}
		i++;
	}
	free_2d_arr(args);
}
