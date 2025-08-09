/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:41:12 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/09 12:08:53 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_token_split_internal(t_token *tokens, char **splited,
			char *expanded, t_token *tmp)
{
	t_token_type	type;
	int				i;
	int				flag;
	int				size;

	size = ft_strlen(expanded) - 1;
	type = tokens->type;
	tokens->value = ft_strdup(splited[0]);
	tokens->next = NULL;
	i = 1;
	flag = 0;
	if (tokens->is_fullstring == 1)
	{
		tokens->is_fullstring = 0;
		flag = 1;
	}
	while (splited && splited[i])
		add_token(&tokens, splited[i++], type, 0);
	if (expanded[size] == ' ')
		flag = 0;
	while (tokens->next)
		tokens = tokens->next;
	if (flag == 1)
		tokens->is_fullstring = 1;
	tokens->next = tmp;
}
