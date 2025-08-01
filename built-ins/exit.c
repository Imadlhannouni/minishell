/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:54:58 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/01 10:54:29 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

static int	ft_isdigit(int a)
{
	if (a >= '0' && a <= '9')
		return (1);
	return (0);
}
int ft_isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(ft_isdigit(str[i]) || str[i] == '-' || str[i] == '+' || is_space(str[i])))
			return (0);
		i++;
	}
	return 1;
}



long	ft_atoi(const char *str)
{
	long	val;
	int	sign;
	int	i;

	i = 0;
	val = 0;
	sign = 1;
	while (is_space(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		// if (!ft_isdigit(str[i]))
		// 	return (-1);
		if (val > 9223372036854775807 && str[i] < '7' && sign == 1)
			return -1;
		val = val * 10 + (str[i] - 48);
		if (val > 9223372036854775807)
			return -1;
		i++;
	}
	return ((long)(val * sign));
}

void	exit_shell(t_exe *var)
{
	long i;

	i = 0;
	if (var_num(var->arr) > 2)
	{
		putstr_fd(" too many arguments\n", 2);
		ft_malloc(0, 1);
		exit(1);
	}
	if ((var->arr)[1] && !ft_isnum((var->arr)[1]))
	{
		putstr_fd(" numeric argument required\n", 2);
		ft_malloc(0, 1);
		exit(2);
	}
	// if (!(var->arr)[1])
	// 	i = ft_atoi(*(collect->exit_code));
	else
		i = ft_atoi((var->arr)[1]);
	// if (i < 0)
	// {
	// 	putstr_fd("Numeric argument required\n", 2);
	// 	exit_free(collect, 2);
	// }
	ft_malloc(0, 1);
	putstr_fd("exit\n", 1);
	exit((i % 256));
}
