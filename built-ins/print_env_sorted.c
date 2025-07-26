/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env_sorted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:08 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/25 21:33:41 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void putstr_fd(char *str, int fd)
{
	int d;
	
	d = write(fd, str, ft_strlen(str));
	if (d < 0)
	{
		perror("Invalid fd\n");
		exit(1);
	}
}

static void put_format(char **var)
{
	putstr_fd("declare -x ", 1);
	if (var[0])
		putstr_fd(var[0], 1);
	if (*var[1])
	{
		write(1, "=\"", 1);
		putstr_fd(var[1], 1);
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

static char **sort_env(char **env, char **arr)
{
	int i;
	int len;
	char *temp;
	char **clone;

	i = 0;
	len = var_num(env) + var_num(arr);
	clone = merge_arr(env, arr);
	if (!clone)
		exit(1);
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
	return clone;
}

int	print_sorted(char **env, char **arr)
{
	int i;
	char **var;
	char **merged_arr;
	
	i = 0;
	merged_arr = sort_env(env, arr);
	if (!merged_arr)
		exit(1) ;
	while (merged_arr[i])
	{
		var = spec_split(merged_arr[i]);
		if (!var)
			exit(1);
		put_format(var);
		i++;
	}
	free_2d_arr(merged_arr);
	return 0;
}
