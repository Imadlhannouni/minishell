/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:16 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/04 14:57:30 by abbenmou         ###   ########.fr       */
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

	clone = ft_malloc((var_num(env) + 2) * sizeof(char*), 0);
	while (env[i])
	{
		clone[i] = ft_strdup(env[i]);
		i++;
	}
	if (!arg[1] || !*arg[1])
		clone[i++] = ft_strdup(arg[0]);
	else
		clone[i++] = join_strings(arg[0], "=", arg[1]);
	clone[i] = NULL;
	i = 0;
	return clone;
}

char *fill_word(int start, int end, char *str)
{
	char *s;
	int i = 0;

	s = ft_malloc((end - start + 2) * sizeof(char), 0);
	while (i < end - start)
	{
		s[i] = str[start + i];
		i++;
	}
	s[i] = '\0';
	return s;
}

static void export_helper(char ***env, char *args, char **arg)
{
	if (check_existence(*env, arg[0]))
	{
		if (!check_equ(args))
			replace_variable(env, arg);
	}
		else
			*env = add_var(*env, arg);	
}

int	export(char ***env, char **args)
{
	char	**arg;
	int		j;
	int		i;

	i = -1;
	j = 0;
	arg = NULL;
	if (*args == NULL)
		return (print_sorted(*env));
	while (args[++i])
	{
		if (args[i])
			arg = spec_split(args[i]);
		if (!check_var(arg[0]))
		{
			putstr_fd("export : not a valid identifier\n", 2);
			j = 1;
			continue;
		}
		else
			export_helper(env, args[i], arg);
	}
	return j;
}
