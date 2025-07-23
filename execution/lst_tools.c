/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:36 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/23 22:50:37 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_exe *creat_node(t_token *tok)
{
	t_exe *node;

	if (!tok)
		return NULL;
	node = malloc(sizeof(t_exe));
	if (!node)
		return NULL;
	node->arr = NULL;
	node->in_red_file = NULL;
	node->out_red_file = NULL;
	node->in_red_type = 0;
	node->out_red_type = 0;
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

void	free_t_exe(t_exe **var)
{
	t_exe *tmp;

	while (*var)
	{
		tmp = *var;
		*var = (*var)->next;
		free_2d_arr(tmp->arr);
		if (tmp->in_red_file)
			free(tmp->in_red_file);
		if (tmp->out_red_file)
			free(tmp->out_red_file);
		free(tmp);
	}
}

int	fill_redirection(t_exe *var, t_token *tok)
{
	if (!(tok->heredoc != 0 || tok->inp_red != 0
		|| tok->out_app != 0 || tok->out_red != 0))
		return 1;
	if (tok->heredoc != 0 || tok->inp_red != 0)
	{
		var->in_red_type = 1;
		if (var->in_red_file)
			free(var->in_red_file);
		var->in_red_file = ft_strdup(tok->value);
		if (!var->in_red_file)
			return 0;
	}
	else if (tok->out_app != 0 || tok->out_red)
	{
		if (tok->out_app != 0)
			var->out_red_type = 1;
		else if (tok->out_red != 0)
			var->out_red_type = 2;
		if (var->out_red_file)
			free(var->out_red_file);
		var->out_red_file = ft_strdup(tok->value);
		if (!var->out_red_file)
			return 0;
	}
	return 1;
}

int	group_2d_arr(t_exe *var,t_token *tok)
{
	t_token	*temp;
	int i;

	i = 0;
	temp = tok;
	(var)->arr = malloc((count_args(tok) + 1) * sizeof(char*));
	if (!(var)->arr)
		return 0;
	while (temp)
	{
		if (((temp->inp_red == 0) && (temp->heredoc == 0)
			&& (temp->out_red == 0) && (temp->out_app == 0))
			&& temp->value)
			(var)->arr[i++] = ft_strdup(temp->value);
		if (!(var)->arr[i - 1])
			return (free_arr((var)->arr, i - 2), 0);
		if(!fill_redirection(var, temp))
			return (free_2d_arr((var)->arr), 0);
		temp = temp->next;
	}
	(var)->arr[i] = NULL;
	return 1;
}
