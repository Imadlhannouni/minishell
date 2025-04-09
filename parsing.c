/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:02:17 by ilhannou          #+#    #+#             */
/*   Updated: 2025/04/09 18:29:41 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_simple_quote(t_token *tokens, t_token_type type, int i, char *line)
{
	int		start;
	int		end;
	char	*value;
	char	*fullvalue;

	type = TOKEN_SIMPLE_QUOTE;

	i++;
	start = i;

	while (line[i] && line[i] != '\'')
		i++;
	end = i;

	if (line[i] == '\'')
		i++;

	value = substrdup(start, end, line);
	if (!value)
		return i;
	if (line[i] && line[i] != ' ' && line[i] != '\t' &&
		line[i] != '|' && line[i] != '<' && line[i] != '>')
		i = after_quote(i, line, value, &fullvalue);
	else
		fullvalue = value;

	add_token(&tokens, fullvalue, type);
	return i;
}



int	is_double_quote(t_token *tokens, t_token_type type, int i, char *line)
{
	int	start;
	int	end;

	type = TOKEN_DOUBLE_QUOTE;
	i++;
	start = i;
	while (line[i] && line[i] != '"')
		i++;
	end = i;
	if (line[i] == '"')
		i++;
	add_token(&tokens, substrdup(start, end, line), type);
	return (i);
}

int	is_directions(t_token *tokens, t_token_type type, int i, char *line)
{
	int	start;
	int	end;

	type = TOKEN_REDIRECTION;
	start = i;
	end = i + 1;
	if ((line[i] == '<' && line[i + 1] == '<') || (line[i] == '>' && line[i + 1] == '>'))
	{
		i++;
		end = i + 1;
	}
	i++;
	add_token(&tokens, substrdup(start, end, line), type);
	return (i);
}

int	is_pipe(t_token *tokens, t_token_type type, int i, char *line)
{
	int	start;
	int	end;

	type = TOKEN_PIPE;
	start = i;
	end = i + 1;
	i++;
	add_token(&tokens, substrdup(start, end, line), type);
	return (i);
}

int	is_word(t_token *tokens, t_token_type type, int i, char *line)
{
	t_token	*last;
	int		start;
	int		end;

	type = TOKEN_WORD;
	start = i;
	while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\'' && line[i] != '\0')
		i++;
	end = i;
	add_token(&tokens, substrdup(start, end, line), type);
	last = ft_lstlast(tokens);
	if (last && (
		ft_strncmp(last->value, "echo", 4) == 0 ||
		ft_strncmp(last->value, "cd", 3) == 0 ||
		ft_strncmp(last->value, "ls", 3) == 0 ||
		ft_strncmp(last->value, "pwd", 4) == 0 ||
		ft_strncmp(last->value, "export", 7) == 0 ||
		ft_strncmp(last->value, "unset", 6) == 0 ||
		ft_strncmp(last->value, "env", 4) == 0 ||
		ft_strncmp(last->value, "exit", 5) == 0
	))
		last->type = TOKEN_CMD;
	return (i);
}
