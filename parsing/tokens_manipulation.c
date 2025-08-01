/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_manipulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:49:01 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/01 11:01:00 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_token(t_token **head, char *value, t_token_type type, int is_full)
{
	t_token	*new;
	t_token	*tmp;

	new = (t_token*)ft_malloc(sizeof(t_token), 0);
	new->type = type;
	new->value = value;
	new->heredoc = 0;
	new->inp_red = 0;
	new->out_app = 0;
	new->out_red = 0;
	new->expand = 0;
	new->is_fullstring = is_full;
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static void	concat_fullstring_flags(t_token *start, t_token *curr)
{
	if (curr->out_red == 1)
		start->out_red = 1;
	else if (curr->inp_red == 1)
		start->inp_red = 1;
	else if (curr->out_app == 1)
		start->out_app = 1;
	else if (curr->heredoc == 1)
		start->heredoc = 1;
}

static t_token	*free_fullstring_tokens(t_token *curr)
{
	t_token	*to_free;

	while (curr && curr->is_fullstring == 1)
	{
		to_free = curr;
		curr = curr->next;
	}
	if (curr && curr->is_fullstring == 0)
	{
		to_free = curr;
		curr = curr->next;
	}
	return (curr);
}

t_token	*concat_fullstring(t_token *start, t_token **next)
{
	char	*joined;
	char	*tmp;
	t_token	*curr;

	curr = start;
	joined = ft_strdup(curr->value);
	while (curr->is_fullstring == 1 && curr->next)
	{
		curr = curr->next;
		tmp = joined;
		joined = ft_strjoin(tmp, curr->value);
	}
	concat_fullstring_flags(start, curr);
	curr = start->next;
	curr = free_fullstring_tokens(curr);
	start->value = joined;
	start->next = curr;
	if (next)
		*next = curr;
	return (start);
}
