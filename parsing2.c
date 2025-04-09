/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 11:08:41 by ilhannou          #+#    #+#             */
/*   Updated: 2025/04/09 15:21:20 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_cmds_var(t_token **tokens, t_token_type type, int i, char *line)
{
	int	start;
	int	is_var;
	int	end;

	is_var = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	start = i;
	while (line[i] != ' ' && line[i] != '\t' && line[i] != '\0')
	{
		type = TOKEN_CMD;
		if (line[i] == '=')
			is_var = 1;
		i++;
	}
	end = i;
	if (is_var == 1)
		add_token(tokens, substrdup(start, end, line), TOKEN_VAR);
	else
		add_token(tokens, substrdup(start, end, line), type);
	return (end);
}
