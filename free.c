/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:11:46 by ilhannou          #+#    #+#             */
/*   Updated: 2025/05/25 22:27:33 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->value)
		{
			free(tokens->value);
		}
		tokens = tokens->next;
	}
	free(tokens);
}
