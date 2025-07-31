/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:38:33 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/31 16:55:39 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_cmd_not_found(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

#define ERR_NOT_REQ "minishell: syntax error: not required character\n"
#define ERR_UNCLOSED "minishell: syntax error: unclosed quote\n"
#define ERR_PIPE "minishell: syntax error near unexpected token `|'\n"
#define ERR_MISSFILE "minishell: syntax error near unexpected token `newline'\n"

static int	skip_spaces(char *s, int i)
{
	while (s[i] && (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13)))
		i++;
	return (i);
}

static int	skip_word(char *s, int i)
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

static int	check_basics(char *line)
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
		{
			ft_putstr_fd(ERR_NOT_REQ, 2);
			return (1);
		}
		if (!dq && line[i] == '\'')
			sq = !sq;
		else if (!sq && line[i] == '"')
			dq = !dq;
		i++;
	}
	if (sq || dq)
		return (ft_putstr_fd(ERR_UNCLOSED, 2), 1);
	return (0);
}

static int	handle_pipe_error(char *s, int i, int has_cmd)
{
	i = skip_spaces(s, i + 1);
	if (!has_cmd || !s[i] || s[i] == '|')
	{
		ft_putstr_fd(ERR_PIPE, 2);
		return (1);
	}
	return (0);
}

static int	handle_redir_token_error(char *s, int i)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putchar_fd(s[i], 2);
	ft_putstr_fd("'\n", 2);
	return (1);
}

static int	handle_redir_error(char *s, int *i_ptr)
{
	char	c;
	int		cnt;
	int		i;

	i = *i_ptr;
	c = s[i];
	cnt = 0;
	if (s[i] == '<' && s[i + 1] == '>')
		return (ft_putstr_fd(ERR_MISSFILE, 2), 1);
	while (s[i] == c)
	{
		cnt++;
		i++;
	}
	if (cnt > 2)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putchar_fd(s[i - 1], 2);
		return (ft_putstr_fd("'\n", 2), 1);
	}
	i = skip_spaces(s, i);
	if (!s[i])
		return (ft_putstr_fd(ERR_MISSFILE, 2), 1);
	if (s[i] == '|' || s[i] == '<' || s[i] == '>')
		return (handle_redir_token_error(s, i));
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
	{
		ft_putstr_fd(ERR_PIPE, 2);
		return (1);
	}
	if (check_operators_loop(s, &i, &has_cmd, &expect_word))
		return (1);
	if (expect_word)
	{
		ft_putstr_fd(ERR_MISSFILE, 2);
		return (1);
	}
	return (0);
}

int	handle_errors(char *line, char *exit_code)
{
	if (!line)
		return (0);
	if (check_basics(line))
	{
		free(exit_code);
		exit_code = ft_strdup("2");
		return (1);
	}
	if (check_operators(line))
	{
		free(exit_code);
		exit_code = ft_strdup("2");
		return (1);
	}
	return (0);
}
