/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:36 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/31 21:45:51 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_exe *creat_node(t_token *tok)
{
	t_exe *node;

	if (!tok)
		return NULL;
	node = (t_exe*)ft_malloc(sizeof(t_exe), 0);
	node->arr = NULL;
	node->redirections = NULL;
	if (!group_2d_arr(node,tok))
		return (free(node), NULL);
	node->next = NULL;
	return node;
}

void add_node(t_exe **lst, t_exe *node)
{
	t_exe *temp;

	if (!lst || !node)
		return;
	if (!*lst)
	{
		*lst = node;
		return;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = node;
}

void	free_red(t_red *help)
{
	t_red *tmp1;
	
	while (help)
	{
		tmp1 = help;
		help = help->next;
		free(tmp1->file);
		free(tmp1);
	}
}

void	free_t_exe(t_exe **var)
{
	t_exe *tmp;

	while (*var)
	{
		tmp = *var;
		*var = (*var)->next;
		free_2d_arr(tmp->arr);
		if (tmp->redirections)
			free_red(tmp->redirections);
		free(tmp);
	}
}
int	is_redirection(t_token *tok)
{
	if (((tok->inp_red == 0) && (tok->heredoc == 0)
			&& (tok->out_red == 0) && (tok->out_app == 0))
			&& tok->value)
			return 0;
	return 1;
}
int	group_2d_arr(t_exe *var,t_token *tok)
{
	t_token	*temp;
	int i;

	i = 0;
	temp = tok;
	(var)->arr = (char**)ft_malloc((count_args(tok) + 1) * sizeof(char*), 0);
	while (temp)
	{
		if (!is_redirection(temp))
			(var)->arr[i++] = ft_strdup(temp->value);
		else
		{
			if(!fill_redirection(var, temp))
				return (free_2d_arr((var)->arr), 0);
		}
		temp = temp->next;
	}
	(var)->arr[i] = NULL;
	return 1;
}
