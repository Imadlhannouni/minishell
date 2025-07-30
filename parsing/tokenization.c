/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:02:17 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/30 17:13:03 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	apply_flag_to_token(t_token *last, int *flag, int set_expand)
{
	if (*flag == 1)
		last->heredoc = 1;
	else if (*flag == 2)
		last->out_app = 1;
	else if (*flag == 3)
		last->inp_red = 1;
	else if (*flag == 4)
		last->out_red = 1;
	if (*flag > 0)
	{
		*flag = 0;
		last->type = TOKEN_PATH;
		if (set_expand)
			last->expand = 1;
	}
}

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
		add_token(tokens, value, type, 1);
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
		add_token(tokens, substrdup(start, end, line), type, 1);
	else
		add_token(tokens, substrdup(start, end, line), type, 0);
	last = ft_lstlast(*tokens);
	apply_flag_to_token(last, flag, 0);
	return (i);
}

int	is_directions(int i, char *line, int *flag)
{
	if (line[i] == '>' && line[i + 1] != '>')
	{
		*flag = 4;
		i++;
	}
	else if (line[i] == '<' && line[i + 1] != '<')
	{
		*flag = 3;
		i++;
	}
	else if (line[i] == '>' && line[i + 1] == '>')
	{
		*flag = 2;
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

int	is_word(t_token **tokens, int i, char *line, int *flag)
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
		add_token(tokens, substrdup(start, end, line), type, 1);
	else
		add_token(tokens, substrdup(start, end, line), type, 0);
	last = ft_lstlast(*tokens);
	apply_flag_to_token(last, flag, 0);
	return (i);
}
