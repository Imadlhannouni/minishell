/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:11:46 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/19 21:22:09 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tokens(t_token *tokens)
{
    t_token	*tmp;

	if (!tokens)
		return;
    while (tokens)
    {
        tmp = tokens;
        if (tokens->value)
            free(tokens->value);
		// if (tokens->heredoc_filename)
		// 	free(tokens->heredoc_filename);
        tokens = tokens->next;
        free(tmp);
    }
}


void	free_pipes(t_pipe **pipes)
{
    t_pipe	*tmp;

    while (*pipes)
    {
        tmp = *pipes;
        *pipes = (*pipes)->nextpipe;
        free_tokens(tmp->full_cmd);
        free(tmp);
    }
}
