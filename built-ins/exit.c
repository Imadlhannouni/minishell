/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:54:58 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/30 23:38:55 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_free(t_free *collect, int exit_code)
{
	if (*(collect->env))
		free_2d_arr(*(collect->env));
	if (collect->exe)
		free_t_exe(&(collect->exe));
	if (collect->fd)
		free(collect->fd);
	if (collect->pid)
		free(collect->pid);
	if (*(collect->no_val))
		free_2d_arr(*(collect->no_val));
	if (collect->pipes)
		free_pipes(&(collect->pipes));
	if ((collect->exit_code) && *(collect->exit_code))
		free(*(collect->exit_code));
	exit(exit_code % 256);
}

static int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}
int ft_isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!((str[i] >= 48 && str[i] <= 57)
			|| str[i] == '-' || str[i] == '+' || is_space(str[i])))
			return (0);
		i++;
	}
	return 1;
}

static int	ft_isdigit(int a)
{
	if (a >= '0' && a <= '9')
		return (1);
	return (0);
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
		if (!ft_isdigit(str[i]))
			return (-1);
		val = val * 10 + (str[i] - 48);
		if (val > 9223372036854775807)
			return -1;
		i++;
	}
	return ((long)(val * sign));
}

void	exit_shell(t_exe *var, t_free *collect)
{
	long i;

	i = 0;
	if (!(var->arr)[1] || !*(var->arr)[1])
	{
		close(collect->fds[0]);
		close(collect->fds[1]);
		exit_free(collect, 1);
	}
	if (!ft_isnum((var->arr)[1]))
	{
		putstr_fd("Numeric argument required\n", 2);
		exit_free(collect, 2);
	}
	i = ft_atoi((var->arr)[1]);
	if (i < 0)
	{
		putstr_fd("Numeric argument required\n", 2);
		exit_free(collect, 2);
	}
	close(collect->fds[0]);
	close(collect->fds[1]);
	if (*(collect->env))
		free_2d_arr(*(collect->env));
	if (collect->exe)
		free_t_exe(&(collect->exe));
	if (*(collect->no_val))
		free_2d_arr(*(collect->no_val));
	if (collect->pipes)
		free_pipes(&(collect->pipes));
	if ((collect->exit_code) && *(collect->exit_code))
		free(*(collect->exit_code));
	putstr_fd("exit\n", 1);
	exit(i % 256);
}
