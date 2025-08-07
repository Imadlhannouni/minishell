/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:55:49 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/07 12:41:54 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_args(t_token *tok)
{
	t_token	*temp;
	int		cpt;

	if (!tok)
		return (0);
	temp = tok;
	cpt = 0;
	while (temp != NULL)
	{
		if ((temp->inp_red == 0) && (temp->heredoc == 0)
			&& (temp->out_red == 0) && (temp->out_app == 0))
			cpt++;
		temp = temp->next;
	}
	return (cpt);
}

void	group_pipes(t_pipe *pipes, t_exe **var)
{
	t_pipe	*tmp;
	t_exe	*node;

	if (!pipes)
		return ;
	tmp = pipes;
	while (tmp)
	{
		node = creat_node(tmp->full_cmd);
		add_node(var, node);
		tmp = tmp->nextpipe;
	}
}

int	ft_isdigit(int a)
{
	if (a >= '0' && a <= '9')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	count_pipes(t_pipe *pipes)
{
	t_pipe	*temp;
	int		cpt;

	temp = pipes;
	cpt = 0;
	while (temp)
	{
		cpt++;
		temp = temp->nextpipe;
	}
	return (cpt);
}
