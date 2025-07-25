/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_manipulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:49:01 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/25 16:12:17 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_token(t_token **head, char *value, t_token_type type, int is_full)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
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
		free(to_free->value);
		free(to_free);
	}
	if (curr && curr->is_fullstring == 0)
	{
		to_free = curr;
		curr = curr->next;
		free(to_free->value);
		free(to_free);
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
	while (curr->is_fullstring == 1)
	{
		curr = curr->next;
		tmp = joined;
		joined = ft_strjoin(tmp, curr->value);
		free(tmp);
	}
	concat_fullstring_flags(start, curr);
	curr = start->next;
	curr = free_fullstring_tokens(curr);
	free(start->value);
	start->value = joined;
	start->next = curr;
	if (next)
		*next = curr;
	return (start);
}
