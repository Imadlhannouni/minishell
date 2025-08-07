/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:36 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/07 12:24:06 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_exe	*creat_node(t_token *tok)
{
	t_exe	*node;

	if (!tok)
		return (NULL);
	node = ft_malloc(sizeof(t_exe), 0);
	node->arr = NULL;
	node->redirections = NULL;
	if (!group_2d_arr(node, tok))
		return (NULL);
	node->next = NULL;
	return (node);
}

void	add_node(t_exe **lst, t_exe *node)
{
	t_exe	*temp;

	if (!lst || !node)
		return ;
	if (!*lst)
	{
		*lst = node;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = node;
}

int	group_2d_arr(t_exe *var, t_token *tok)
{
	t_token	*temp;
	int		i;

	i = 0;
	temp = tok;
	(var)->arr = ft_malloc((count_args(tok) + 1) * sizeof(char *), 0);
	while (temp)
	{
		if (!is_redirection(temp))
			(var)->arr[i++] = ft_strdup(temp->value);
		else
		{
			if (!fill_redirection(var, temp))
				return (0);
		}
		temp = temp->next;
	}
	(var)->arr[i] = NULL;
	return (1);
}

int	is_redirection(t_token *tok)
{
	if (((tok->inp_red == 0) && (tok->heredoc == 0)
			&& (tok->out_red == 0) && (tok->out_app == 0))
		&& tok->value)
		return (0);
	return (1);
}

int	fill_redirection(t_exe *var, t_token *tok)
{
	t_red	*new_red;

	if (!(tok->heredoc != 0 || tok->inp_red != 0
			|| tok->out_app != 0 || tok->out_red != 0))
		return (1);
	new_red = create_redirection(tok);
	add_redirection(&(var->redirections), new_red);
	return (1);
}
