/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:35:41 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/03 14:36:34 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

int	skip_spaces(char *s, int i)
{
	while (s[i] && (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13)))
		i++;
	return (i);
}

int	skip_word(char *s, int i)
{
	int	sq;
	int	dq;

	sq = 0;
	dq = 0;
	while (s[i])
	{
		if (!dq && s[i] == '\'')
			sq = !sq;
		else if (!sq && s[i] == '"')
			dq = !dq;
		else if (!sq && !dq && (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13)
				|| s[i] == '|' || s[i] == '<' || s[i] == '>'))
			break ;
		i++;
	}
	return (i);
}

int	check_basics(char *line)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (line[i])
	{
		if (!sq && !dq && (line[i] == '\\' || line[i] == ';'))
			return (ft_putstr_fd("minishell: not required characters\n", 2), 1);
		if (!dq && line[i] == '\'')
			sq = !sq;
		else if (!sq && line[i] == '"')
			dq = !dq;
		i++;
	}
	if (sq || dq)
		return (ft_putstr_fd("minishell: syntax error unclosed quotes\n", 2),
			1);
	return (0);
}

int	handle_pipe_error(char *s, int i, int has_cmd)
{
	i = skip_spaces(s, i + 1);
	if (!has_cmd || !s[i] || s[i] == '|')
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	return (0);
}
