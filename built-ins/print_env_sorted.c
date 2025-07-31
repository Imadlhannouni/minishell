/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env_sorted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:08 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/31 22:15:11 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void putstr_fd(char *str, int fd)
{
	int d;
	
	d = write(fd, str, ft_strlen(str));
	if (d < 0)
	{
		perror("Invalid fd\n");
		ft_malloc(0, 1);
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
		write(1, "=\"", 2);
		putstr_fd(var[1], 1);
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

static char **sort_env(char **env)
{
	int i;
	int len;
	char *temp;
	char **clone;

	i = 0;
	len = var_num(env);
	clone = clone_env(env);
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

int	print_sorted(char **env)
{
	int i;
	char **var;
	char **sorted_arr;
	
	i = 0;
	sorted_arr = sort_env(env);
	while (sorted_arr[i])
	{
		var = spec_split(sorted_arr[i]);
		put_format(var);
		free_2d_arr(var);
		i++;
	}
	free_2d_arr(sorted_arr);
	return 0;
}
