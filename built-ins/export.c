/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:16 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/30 23:27:23 by abbenmou         ###   ########.fr       */
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

static void init_no_val(char ***no_val, t_free *collect, char *arg)
{
	*no_val = malloc(2 * sizeof(char*));
	if (!*no_val)
	exit_free(collect, 1);
	(*no_val)[0] = ft_strdup(arg);
	if (!(*no_val)[0])
		exit_free(collect, 1);
	(*no_val)[1] = NULL;
}

static void fill_no_val(char ***no_val, char *arg, t_free *collect)
{
	char **new;
	int i;

	i = 0;
	new = malloc((var_num(*no_val) + 2) * sizeof(char *));
	if (!new)
		exit_free(collect, 1);
	while ((*no_val)[i])
	{
		new[i] = ft_strdup((*no_val)[i]);
		if (!new[i])
			return (free_arr(new, i - 1),exit_free(collect, 1));
		i++;
	}
	new[i++] = ft_strdup(arg);
	if (!new[i - 1])
		return (free_arr(new, i - 2),exit_free(collect, 1));
	new[i] = NULL;
	free_2d_arr(*no_val);
	*no_val = new; 
}

void store_no_val(char ***no_val, char ***env, char *arg, t_free *collect)
{
	if (*no_val == NULL)
		init_no_val(no_val, collect, arg);
	else if (check_existence(*env, arg) || check_existence(*no_val, arg))
		return ;
	else
		fill_no_val(no_val, arg, collect);
}

int	export(char ***env, char **args, char ***no_val, t_free *collect)
{
	char	**arg = NULL;
	int		i;

	i = -1;
	if (*args == NULL)
		return (print_sorted(*env, *no_val, collect));
	while (args[++i])
	{
		if (args[i])
			arg = spec_split(args[i]);
		if (!check_var(arg[0]))
			return (putstr_fd("export : not a valid identifier\n", 2), 1);
		if (!*arg[1])
			store_no_val(no_val, env, arg[0], collect);
		else
		{
			if (check_existence(*env, arg[0]))
				replace_variable(env, arg);
			else
				*env = add_var(*env, arg);
		}
		free_2d_arr(arg);
	}
	return 0;
}
