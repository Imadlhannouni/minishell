#include "../minishell.h"

char **merge_arr(char **env, char **arr)
{
	int i = 0, j = 0;
	char **new = NULL;

	new = malloc((var_num(env) + var_num(arr) + 1) * sizeof(char*));
	if (!new)
		return NULL;
	while (env[i])
	{
		new[j++] = ft_strdup(env[i++]);
	}
	i = 0;
	if (arr != NULL)
	{
		while (arr[i])
		{
			new[j++] = ft_strdup(arr[i++]);
		}
	}
	new[j] = NULL;
	return new;
}
void put_str(char *str)
{
	int i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	
}

void	print_sorted(char **env, char **arr)
{
	int i = 0;
	char *temp;
	int len = var_num(env) + var_num(arr);
	char **clone = merge_arr(env, arr);
	while (i < len - 1)
	{
		if (strcmp(clone[i] ,clone[i + 1]) > 0)
		{
			temp = clone[i];
			clone[i] = clone[i + 1];
			clone[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
	i = 0;
	char **var;
	while (clone[i])
	{
		var = spec_split(clone[i]);
		if (!var)
			return;
		put_str("declare -x ");
		if (var[0])
			put_str(var[0]);
		if (*var[1])
		{
			write(1, "=\"", 2);
			put_str(var[1]);
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		i++;
	}
	free_2d_arr(clone);
}
