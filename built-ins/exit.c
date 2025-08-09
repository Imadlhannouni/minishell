/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:54:58 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/09 12:02:58 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (str[0] == 0)
		return (0);
	while (str[i])
	{
		if (!(ft_isdigit(str[i]) || str[i] == '-'
				|| str[i] == '+' || is_space(str[i])))
			return (0);
		if (ft_isdigit(str[i]))
			break ;
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	check_val(long val, char c, int sign)
{
	if (sign < 0)
	{
		if ((-val < (LONG_MIN + (c - 48)) / 10))
		{
			putstr_fd(" numeric argument required\n", 2);
			exit_free(2);
		}
	}
	else
	{
		if ((val > (LONG_MAX - (c - 48)) / 10))
		{
			putstr_fd(" numeric argument required\n", 2);
			exit_free(2);
		}
	}
}

long	ft_atoi(const char *str)
{
	long	val;
	int		sign;
	int		i;

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
		check_val(val, str[i], sign);
		val = val * 10 + (str[i++] - 48);
	}
	return ((long)(val * sign));
}

static void	close_opened(t_help *help)
{
	if (!help->child)
	{
		close(help->std_fd[0]);
		close(help->std_fd[1]);
	}
}

int	exit_shell(t_exe *var, t_help *help)
{
	long	i;

	i = 0;
	if (!help->child)
		putstr_fd("exit\n", 2);
	if (var->arr[1] == NULL)
		return (close_opened(help), exit_free(ft_atoi(*(help->exit_code))), 0);
	if (var_num(var->arr) > 2)
	{
		if (!ft_isnum(var->arr[1]))
		{
			putstr_fd("Minishell : numeric argument required\n", 2);
			return (close_opened(help), exit_free(2), 1);
		}
		else if (ft_isnum(var->arr[1]))
			return (putstr_fd("Minishell : too many arguments\n", 2), 1);
	}
	if (var->arr[1] && !ft_isnum(var->arr[1]))
	{
		putstr_fd("Minishell : numeric argument required\n", 2);
		return (close_opened(help), exit_free(2), 1);
	}
	i = ft_atoi((var->arr)[1]);
	return (close_opened(help), exit_free((i % 256)), 0);
}
