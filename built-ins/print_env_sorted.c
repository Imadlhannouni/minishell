/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env_sorted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:08 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/08 18:46:49 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	putstr_fd(char *str, int fd)
{
	int	d;

	d = write(fd, str, ft_strlen(str));
	if (d < 0)
	{
		perror("Invalid fd\n");
		exit_free(1);
	}
}

static void	put_format(char **var)
{
	printf("declare -x ");
	if (var[0])
		printf("%s=", var[0]);
	if (var[1])
	{
		printf("\"%s\"", var[1]);
	}
	printf("\n");
}

static char	**sort_env(char **env)
{
	int		i;
	int		len;
	char	*temp;
	char	**clone;

	i = 0;
	len = var_num(env);
	clone = clone_env(env);
	while (i < len - 1)
	{
		if (strcmp(clone[i], clone[i + 1]) > 0)
		{
			temp = clone[i];
			clone[i] = clone[i + 1];
			clone[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
	return (clone);
}

int	print_sorted(char **env)
{
	int		i;
	char	**var;
	char	**sorted_arr;

	i = 0;
	sorted_arr = sort_env(env);
	while (sorted_arr[i])
	{
		var = spec_split(sorted_arr[i]);
		put_format(var);
		i++;
	}
	return (0);
}
