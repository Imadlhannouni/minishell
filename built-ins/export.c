/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:16 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/25 21:32:37 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



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
	free_2d_arr(env);
	return clone;
}

char *fill_word(int start, int end, char *str)
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



int	export(char ***env, char **args, char ***no_val)
{
	char	**arg = NULL;
	int		i;

	i = -1;
	if (*args == NULL)
		return (print_sorted(*env, *no_val));
	while (args[++i])
	{
		if (args[i])
			arg = spec_split(args[i]);
		if (!check_var(arg[0]))
			return (putstr_fd("export : not a valid identifier\n", 2), 1);
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
	}
	return 0;
}
