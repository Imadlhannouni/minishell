/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 11:08:41 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/18 16:13:22 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quoted_part(int *i, char *line, char **part)
{
	char	quote;
	int		start;

	quote = line[(*i)++];
	start = *i;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	*part = substrdup(start, *i, line);
	(*i)++;
	return (*i);
}

static int	handle_unquoted_part(int *i, char *line, char **part)
{
	int	start;

	start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != '<' && line[*i] != '>' && line[*i] != '"'
		&& line[*i] != '\'')
		(*i)++;
	*part = substrdup(start, *i, line);
	return (*i);
}

int	is_cmds_var(t_token **tokens, t_token_type type, int i, char *line)
{
	char	*value;
	char	*part;

	value = NULL;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '|'
		&& line[i] != '<' && line[i] != '>')
	{
		if (line[i] == '"' || line[i] == '\'')
			handle_quoted_part(&i, line, &part);
		else
			handle_unquoted_part(&i, line, &part);
		if (!value)
			value = ft_strdup(part);
		else
			value = ft_strjoin(value, part);
		free(part);
	}
	add_token(tokens, value, TOKEN_CMD, 0);
	return (i);
}

void	is_path(t_pipe *pipe)
{
	t_pipe	*curr_pipe;
	t_token	*curr_token;

	curr_pipe = pipe;
	while (curr_pipe)
	{
		curr_token = curr_pipe->full_cmd;
		while (curr_token)
		{
			if (((curr_token->type == TOKEN_CMD && ft_strcmp(curr_token->value,
							"cd") == 0)
					|| curr_token->type == TOKEN_REDIRECTION)
				&& curr_token->next != NULL)
			{
				if (curr_token->next->type == TOKEN_WORD
					|| curr_token->next->type == TOKEN_SIMPLE_QUOTE
					|| curr_token->next->type == TOKEN_DOUBLE_QUOTE)
				{
					curr_token->next->type = 7;
				}
			}
			curr_token = curr_token->next;
		}
		curr_pipe = curr_pipe->nextpipe;
	}
}

int	is_option(t_token *tokens, t_token_type type, int i, char *line)
{
	int	start;
	int	end;

	type = TOKEN_OPTION;
	start = i;
	if (line[i] == '-')
		i++;
	if (line[i] == 'n')
		i++;
	end = i;
	add_token(&tokens, substrdup(start, end, line), type, 0);
	return (i);
}
