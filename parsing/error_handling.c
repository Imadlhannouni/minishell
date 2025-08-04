/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:38:33 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/03 14:36:17 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_redir_error(char *s, int *i_ptr)
{
	char	c;
	int		cnt;
	int		i;

	i = *i_ptr;
	c = s[i];
	cnt = 0;
	if (s[i] == '<' && s[i + 1] == '>')
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	while (s[i] == c)
	{
		cnt++;
		i++;
	}
	if (cnt > 2)
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	i = skip_spaces(s, i);
	if (!s[i])
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	if (s[i] == '|' || s[i] == '<' || s[i] == '>')
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	*i_ptr = i;
	return (0);
}

static void	update_word_flags(int *i, int *expect_word, int *has_cmd, char *s)
{
	if (*expect_word)
	{
		*i = skip_word(s, *i);
		*expect_word = 0;
		*has_cmd = 1;
	}
	else
	{
		*i = skip_word(s, *i);
		*has_cmd = 1;
	}
	*i = skip_spaces(s, *i);
}

static int	check_operators_loop(char *s, int *i, int *has_cmd,
		int *expect_word)
{
	while (s[*i])
	{
		if (s[*i] == '|' && !(*expect_word))
		{
			if (handle_pipe_error(s, *i, *has_cmd))
				return (1);
			*has_cmd = 0;
			*i = skip_spaces(s, *i + 1);
			continue ;
		}
		else if ((s[*i] == '<' || s[*i] == '>') && !(*expect_word))
		{
			if (handle_redir_error(s, i))
				return (1);
			*expect_word = 1;
			continue ;
		}
		update_word_flags(i, expect_word, has_cmd, s);
	}
	return (0);
}

static int	check_operators(char *s)
{
	int	i;
	int	has_cmd;
	int	expect_word;

	i = skip_spaces(s, 0);
	has_cmd = 0;
	expect_word = 0;
	if (!s[i])
		return (0);
	if (s[i] == '|')
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	if (check_operators_loop(s, &i, &has_cmd, &expect_word))
		return (1);
	if (expect_word)
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	return (0);
}

int	handle_errors(char *line, char **exit_code)
{
	if (!line)
		return (0);
	if (check_basics(line))
	{
		*exit_code = ft_strdup("2");
		return (1);
	}
	if (check_operators(line))
	{
		*exit_code = ft_strdup("2");
		return (1);
	}
	return (0);
}
