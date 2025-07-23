/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:19 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/23 22:51:20 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*join_vars(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	str = NULL;
	i = 0;
	j = 0;
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (NULL);
	if (s1)
	{
		while (s1[i])
			str[j++] = s1[i++];
		str[j++] = ' ';
	}
	i = 0;
	if (s2)
	{
		while (s2[i])
			str[j++] = s2[i++];
	}
	str[j] = '\0';
	return (free(s1), str);
}

void	put_nl(char **str, int flag)
{
	if (!flag)
		printf("%s", *str);
	else
		printf("%s\n", *str);
	free(*str);
}

void	echo(char **arg)
{
	int		i;
	char	*str;
	int		k;

	str = NULL;
	k = 1;
	if (!arg[1])
	{
		write(1, "\n", 1);
		return ;
	}
	else if (arg[1] && !arg[2] && !ft_strcmp(arg[1], "-n"))
		return ;
	i = 1;
	if (!ft_strcmp(arg[1], "-n"))
	{
		k = 0;
		i = 2;
	}
	while (arg[i])
	{
		str = join_vars(str, arg[i]);
		i++;
	}
	put_nl(&str, k);
}
