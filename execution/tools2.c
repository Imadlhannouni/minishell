/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:55:49 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/30 14:58:32 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_args(t_token *tok)
{
	t_token *temp;
	int cpt;

	if (!tok)
		return 0;
	temp = tok;
	cpt = 0;

	while (temp != NULL)
	{
		if ((temp->inp_red == 0) && (temp->heredoc == 0)
			&& (temp->out_red == 0) && (temp->out_app == 0))
			cpt++;
		temp = temp->next;
	}
	return cpt;
}

void group_pipes(t_pipe *pipes, t_exe **var, t_free *collect, char **env)
{
	t_pipe *tmp;

	if (!pipes)
		return;
	collect->pipes = pipes;
	tmp = pipes;
	while (tmp)
	{
		add_node(var,creat_node(tmp->full_cmd));
		tmp = tmp->nextpipe;
	}
	collect->exe = *var;
	collect->env = env;
}