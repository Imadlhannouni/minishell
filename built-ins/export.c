/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:16 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/24 20:03:46 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	s = malloc((end - start + 2) * sizeof(char));
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


char **spec_split(char *str)
{
	char **arr = NULL;
	int i = 0;

	if (!str)
		return NULL;
	arr = malloc(3 * sizeof(char*));
	if (!arr)
		return NULL;
	while (str[i] && str[i] != '=')
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

void store_no_val(char ***no_val, char ***env, char *arg)
{
	int i = 0;
	if (*no_val == NULL)
	{
		*no_val = malloc(2 * sizeof(char*));
		if (!*no_val)
			return;
		(*no_val)[0] = ft_strdup(arg);
		(*no_val)[1] = NULL;
	}
	else if (check_existence(*env, arg) == 1)
		return;
	else
	{
		char **new;
		new = malloc((var_num(*no_val) + 2) * sizeof(char *));
		if (!new)
			return;
		while ((*no_val)[i])
		{
			new[i] = ft_strdup((*no_val)[i]);
			i++;
		}
		new[i++] = ft_strdup(arg);
		new[i] = NULL;
		free_2d_arr(*no_val);
		*no_val = new; 
	}
}

int	check_char(char c)
{
	if (!((c >= 48 && c <= 57) ||
		(c >= 65 && c <= 90) ||
		(c >= 97 && c <= 122) ||
		c == 95 || c == '='))
		return 0;
	return 1; 
}
int check_var(char *str)
{
	int i = 0;
	if (!((str[i] >= 65 && str[i] <= 90) ||
		(str[i] >= 97 && str[i] <= 122) ||
		str[i] == 95 || str[i] == '='))
		return 0;
	while (str[i])
	{
		if (!check_char(str[i]))
			return 0;
		i++;
	}
	return 1;
}

void	export(char ***env, char **args, char ***no_val)
{
	char	**arg = NULL;
	int		i;

	i = 0;
	if (*args == NULL)
		return print_sorted(*env, *no_val);
	while (args[i])
	{
		arg = spec_split(args[i]);
		if (!check_var(arg[0]))
		{
			printf("export : %s : not a valid identifier\n",args[i]);
			return;
		}
		if (!arg[1])
			store_no_val(no_val, env, args[i]);
		else
		{
			if (check_existence(*env, arg[0]))
				replace_variable(env, arg);
			else
				*env = add_var(*env, arg);
			free_2d_arr(arg);
		}
		i++;
	}
}
