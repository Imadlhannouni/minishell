/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_manipulation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:49:01 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/06 21:55:58 by ilhannou         ###   ########.fr       */
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
		if (*flag > 1)
			last->type = TOKEN_PATH;
		if (set_expand)
			last->expand = 1;
	}
}

int	add_token(t_token **head, char *value, t_token_type type, int is_full)
{
	t_token	*new;
	t_token	*tmp;

	new = (t_token *)ft_malloc(sizeof(t_token), 0);
	new->type = type;
	new->value = value;
	new->heredoc = 0;
	new->inp_red = 0;
	new->out_app = 0;
	new->out_red = 0;
	new->expand = 0;
	new->ambigious = 0;
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
	return (1);
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
	while (curr && curr->is_fullstring == 1)
	{
		curr = curr->next;
	}
	if (curr && curr->is_fullstring == 0)
	{
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
	start->type = TOKEN_LINKEDSTRING;
	if (next)
		*next = curr;
	return (start);
}
