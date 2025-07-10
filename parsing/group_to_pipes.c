/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_to_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:40:28 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/10 21:55:54 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	compact_fullstrings(t_token **fullcmd)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*compact;

	curr = *fullcmd;
	prev = NULL;
	while (curr)
	{
		if (curr->is_fullstring == 1)
		{
			compact = concat_fullstring(curr, &curr);
			compact->type = 9;
			if (prev)
				prev->next = compact;
			else
				*fullcmd = compact;
			prev = compact;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

void	add_pipe(t_pipe **head, t_token *fullcmd)
{
	t_pipe	*new;
	t_pipe	*tmp;

	new = malloc(sizeof(t_pipe));
	if (!new)
		return ;
	compact_fullstrings(&fullcmd);
	new->full_cmd = fullcmd;
	new->nextpipe = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->nextpipe)
			tmp = tmp->nextpipe;
		tmp->nextpipe = new;
	}
}

t_pipe	*group_tokens_into_pipes(t_token *tokens)
{
	t_pipe	*pipes;
	t_token	*start;
	t_token	*curr;
	t_token	*prev;

	pipes = NULL;
	start = tokens;
	curr = tokens;
	prev = NULL;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE)
		{
			curr->value = NULL;
			if (prev)
				prev->next = NULL;
			add_pipe(&pipes, start);
			start = curr->next;
		}
		prev = curr;
		curr = curr->next;
	}
	if (start)
		add_pipe(&pipes, start);
	return (pipes);
}

void	print_pipes(t_pipe *pipes)
{
	t_pipe	*curr_pipe;
	t_token	*curr_token;

	curr_pipe = pipes;
	while (curr_pipe)
	{
		printf("=== New Pipe Command ===\n");
		curr_token = curr_pipe->full_cmd;
		while (curr_token)
		{
			printf("%s : %d : %d | inp_red : %d, out_red : %d,  heredoc : %d, out_app : %d\n", curr_token->value, curr_token->type,
				curr_token->is_fullstring, curr_token->inp_red,
				curr_token->out_red, curr_token->heredoc, curr_token->out_app);
			curr_token = curr_token->next;
		}
		curr_pipe = curr_pipe->nextpipe;
	}
}
