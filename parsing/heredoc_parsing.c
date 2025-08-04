/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:36:24 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/04 16:39:34 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_heredoc_token(t_token *current_token, char **clone_envi,
		char **exit_code)
{
	t_token	*compact;

	compact = current_token;
	if (current_token->is_fullstring == 1)
	{
		concat_fullstring(compact, NULL);
		current_token->is_fullstring = 0;
	}
	if (!handle_heredoc_token(current_token, clone_envi, exit_code))
		return (0);
	return (1);
}

int	handle_heredoc_tokens(t_token *token, char **clone_envi, char **exit_code)
{
	while (token)
	{
		if (token->heredoc == 1)
		{
			if (!process_heredoc_token(token, clone_envi, exit_code))
				return (0);
		}
		token = token->next;
	}
	return (1);
}
