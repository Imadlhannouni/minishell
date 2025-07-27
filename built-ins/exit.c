/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:54:58 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/27 12:23:38 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_free(t_free *collect, int exit_code)
{
	if (collect->env)
		free_2d_arr(collect->env);
	if (collect->exe)
		free_t_exe(&(collect->exe));
	if (collect->fd)
		free(collect->fd);
	if (collect->no_val)
		free_2d_arr(collect->no_val);
	if (collect->pid)
		free(collect->pid);
	if (collect->pipes)
		free_pipes(&(collect->pipes));
	if (collect->fds)
	{
		close(collect->fds[0]);
		close(collect->fds[1]);
	}
	putstr_fd("exit\n", 1);
	exit(exit_code % 255);
}

int ft_isnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= 48 && str[i] <= 57))
			return (0);
		i++;
	}
	return 1;
}

static int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	val;
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
	while (str[i] >= '0' && str[i] <= '9')
	{
		val = val * 10 + (str[i] - 48);
		i++;
	}
	return (val * sign);
}

void	exit_shell(t_exe *var, t_free *collect)
{
	int i;

	i = 0;
	if (!(var->arr)[1] || !*(var->arr)[1])
		exit_free(collect, 1);
	if (!ft_isnum((var->arr)[1]))
	{
		putstr_fd("Numeric argument required\n", 2);
		exit_free(collect, 2);
	}
	i = ft_atoi((var->arr)[1]);
	exit_free(collect, i);
}
