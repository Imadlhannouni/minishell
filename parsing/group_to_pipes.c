/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_to_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:40:28 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/05 16:52:24 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_null(t_token **fullcmd, t_token **fullcmd_v)
{
	t_token	*head;

	head = *fullcmd_v;
	if (head == *fullcmd)
	{
		if ((*fullcmd)->value == NULL && (*fullcmd)->next)
			(*fullcmd) = (*fullcmd)->next;
	}
}

void	compact_fullstrings(t_token **fullcmd)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*compact;

	curr = *fullcmd;
	prev = NULL;
	while (curr)
	{
		check_null(&curr, fullcmd);
		if (curr->is_fullstring == 1)
		{
			compact = concat_fullstring(curr, &curr);
			compact->type = TOKEN_LINKEDSTRING;
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

	new = (t_pipe *)ft_malloc(sizeof(t_pipe), 0);
	new->full_cmd = fullcmd;
	new->ambigious = 0;
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

static void	handle_pipe_token(t_pipe **pipes, t_token **start, t_token **curr,
		t_token **prev)
{
	t_token	*next;

	next = (*curr)->next;
	if (*prev)
		(*prev)->next = NULL;
	add_pipe(pipes, *start);
	*start = next;
	*curr = next;
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
			handle_pipe_token(&pipes, &start, &curr, &prev);
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
	if (start)
		add_pipe(&pipes, start);
	return (pipes);
}
