/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 11:08:41 by ilhannou          #+#    #+#             */
/*   Updated: 2025/04/15 16:34:15 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_cmds_var(t_token **tokens, t_token_type type, int i, char *line)
{
	int	start;
	int	is_var;
	int	end;

	is_var = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	start = i;
	while (line[i] != ' ' && line[i] != '\t' && line[i] != '\0')
	{
		type = TOKEN_CMD;
		if (line[i] == '=')
			is_var = 1;
		i++;
	}
	end = i;
	if (is_var == 1)
		add_token(tokens, substrdup(start, end, line), TOKEN_VAR, 0);
	else
		add_token(tokens, substrdup(start, end, line), type, 0);
	return (end);
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
			if (((curr_token->type == TOKEN_CMD && ft_strcmp(curr_token->value, "cd") == 0) 
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
