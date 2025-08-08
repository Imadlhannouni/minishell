/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:02:17 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/08 13:52:55 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_simple_quote(t_token **tokens, int i, char *line, int *flag)
{
	int				start;
	int				end;
	char			*value;
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
	{
		add_token(tokens, value, type, 1);
	}
	else
		add_token(tokens, value, type, 0);
	last = ft_lstlast(*tokens);
	apply_flag_to_token(last, flag, 1);
	return (i);
}

int	is_double_quote(t_token **tokens, int i, char *line, int *flag)
{
	int				start;
	int				end;
	char			*value;
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
	value = substrdup(start, end, line);
	if (line[i] != ' ' && line[i] != '\t' && line[i] != '|' && line[i] != '<'
		&& line[i] != '>' && line[i] != '\0')
	{
		add_token(tokens, value, type, 1);
	}
	else
		add_token(tokens, value, type, 0);
	last = ft_lstlast(*tokens);
	return (apply_flag_to_token(last, flag, 0), i);
}

int	is_directions(int i, char *line, int *flag)
{
	if (line[i] == '>' && line[i + 1] != '>')
		*flag = 4;
	else if (line[i] == '<' && line[i + 1] != '<' && line[i + 1] != '>')
		*flag = 3;
	else if (line[i] == '>' && line[i + 1] == '>')
		*flag = 2;
	else if (line[i] == '<' && line[i + 1] == '<')
		*flag = 1;
	if (*flag == 4 || *flag == 3)
		i++;
	else if (*flag == 1 || *flag == 2)
		i += 2;
	if (line[i] == '<' && line[i + 1] == '>')
	{
		*flag = 3;
		i += 2;
	}
	return (i);
}

int	is_pipe(t_token *tokens, int i, char *line)
{
	int				start;
	int				end;
	char			*value;
	t_token_type	type;

	type = TOKEN_PIPE;
	start = i;
	end = i + 1;
	i++;
	value = substrdup(start, end, line);
	add_token(&tokens, value, type, 0);
	return (i);
}

int	is_word(t_token **tokens, int i, char *line, int *flag)
{
	t_token			*last;
	int				start;
	char			*value;
	int				end;
	t_token_type	type;

	type = TOKEN_WORD;
	start = i;
	while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\''
		&& line[i] != '"' && line[i] != '|' && line[i] != '<' && line[i] != '>'
		&& line[i] != '\0')
	{
		if (line[i] == '$' && i != start)
			break ;
		i++;
	}
	end = i;
	value = substrdup(start, end, line);
	if (line[i] != ' ' && line[i] != '\t' && line[i] != '|' && line[i] != '<'
		&& line[i] != '>' && line[i] != '\0')
		add_token(tokens, value, type, 1);
	else
		add_token(tokens, value, type, 0);
	last = ft_lstlast(*tokens);
	return (apply_flag_to_token(last, flag, 0), i);
}
