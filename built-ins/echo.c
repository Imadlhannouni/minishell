/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:51:19 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/07 12:09:42 by ilhannou         ###   ########.fr       */
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
	str = ft_malloc((ft_strlen(s1) + ft_strlen(s2) + 2), 0);
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
	return (str);
}

void	put_nl(char **str, int flag)
{
	if (!flag)
		putstr_fd(*str, 1);
	else
	{
		putstr_fd(*str, 1);
		putstr_fd("\n", 1);
	}
}

int	check_flag(char *arg)
{
	int i;

	i = 0;
	if (!arg)
		return (0);
	if (arg[i++] != '-')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return 0;
		i++;
	}
	return (1);
}

int	echo(char **arg)
{
	int		i;
	char	*str;
	int		k;

	str = NULL;
	k = 1;
	if (!arg[1])
		return (putstr_fd("\n", 1), 0);
	else if (arg[1] && !arg[2] && check_flag(arg[1]))
		return (0);
	i = 1;
	while (check_flag(arg[i]))
	{
		k = 0;
		i++;
	}
	while (arg[i])
		str = join_vars(str, arg[i++]);
	put_nl(&str, k);
	return (0);
}
