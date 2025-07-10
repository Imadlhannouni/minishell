/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:02:17 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/29 14:48:24 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_simple_quote(t_token *tokens, int i, char *line, int *flag)
{
	int				start;
	int				end;
	char			*value;
	char			*fullvalue;
	t_token			*last;
	t_token_type	type;

	type = TOKEN_SIMPLE_QUOTE;
	i++;
	start = i;
	while (line[i] && line[i] != '\'' && line[i + 1] != '\0')
		i++;
	end = i;
	if (line[i] == '\'')
		i++;
	value = substrdup(start, end, line);
	if (line[i] != ' ' && line[i] != '\t' && line[i] != '|' && line[i] != '<'
		&& line[i] != '>' && line[i] != '\0')
		add_token(&tokens, value, type, 1);
	else
		add_token(&tokens, value, type, 0);
	last = ft_lstlast(tokens);
	if (*flag == 1)
	{
		*flag = 0;
		last->heredoc = 1;
	}
	return (i);
}

int	is_double_quote(t_token *tokens, int i, char *line, int *flag)
{
	int				start;
	int				end;
	t_token_type	type;
	t_token			*last;

	type = TOKEN_DOUBLE_QUOTE;
	i++;
	start = i;
	while (line[i] && line[i] != '"')
		i++;
	end = i;
	if (line[i] == '"')
		i++;
	if (line[i] != ' ' && line[i] != '\t' && line[i] != '|' && line[i] != '<'
		&& line[i] != '>' && line[i] != '\0')
		add_token(&tokens, substrdup(start, end, line), type, 1);
	else
		add_token(&tokens, substrdup(start, end, line), type, 0);
	last = ft_lstlast(tokens);
	if (*flag == 1)
	{
		*flag = 0;
		last->heredoc = 1;
	}
	return (i);
}

int	is_directions(t_token *tokens, int i, char *line, int *flag)
{
	if (line[i] == '>' && line[i + 1] != '>')
	{
		add_redirection(&tokens, 0);
		i++;
	}
	else if (line[i] == '<' && line[i + 1] != '<')
	{
		add_redirection(&tokens, 1);
		i++;
	}
	else if (line[i] == '>' && line[i + 1] == '>')
	{
		add_redirection(&tokens, 2);
		i += 2;
	}
	else if (line[i] == '<' && line[i + 1] == '<')
	{
		*flag = 1;
		i += 2;
	}
	return (i);
}

int	is_pipe(t_token *tokens, int i, char *line)
{
	int				start;
	int				end;
	t_token_type	type;

	type = TOKEN_PIPE;
	start = i;
	end = i + 1;
	i++;
	add_token(&tokens, substrdup(start, end, line), type, 0);
	return (i);
}

int	is_word(t_token *tokens, int i, char *line, int *flag)
{
	t_token			*last;
	int				start;
	int				end;
	t_token_type	type;

	type = TOKEN_WORD;
	start = i;
	while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\''
		&& line[i] != '"' && line[i] != '|' && line[i] != '<' && line[i] != '>'
		&& line[i] != '\0')
		i++;
	end = i;
	if (line[i] != ' ' && line[i] != '\t' && line[i] != '|' && line[i] != '<'
		&& line[i] != '>' && line[i] != '\0')
		add_token(&tokens, substrdup(start, end, line), type, 1);
	else
		add_token(&tokens, substrdup(start, end, line), type, 0);
	last = ft_lstlast(tokens);
	if (*flag == 1)
	{
		*flag = 0;
		last->heredoc = 1;
	}
	return (i);
}
