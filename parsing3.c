/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:46:52 by ilhannou          #+#    #+#             */
/*   Updated: 2025/04/09 18:41:36 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	after_quote(int i, char *line, char *value, char **fullvalue)
{
	int		start;
	int		end;
	char	*value2 = NULL;
	char	*temp;

	while (line[i] == '\'' || line[i] == '"')
		i++;
	start = i;
	while (line[i] && line[i] != ' ' && line[i] != '\t' &&
		   line[i] != '|' && line[i] != '<' && line[i] != '>')
		i++;
	end = i;
	value2 = substrdup(start, end, line);
	if (!value2)
		return i;
	temp = ft_strjoin(value, value2);
	free(value);
	free(value2);
	if (!temp)
		return i;
	*fullvalue = temp;
	return i;
}


int	after_word(int i, char *line, char *value, char **fullvalue)
{
	int		start;
	int		end;
	char	*value2 = NULL;

	start = i;
	while (line[i] && line[i] != ' ' && line[i] != '\t' &&
		   line[i] != '|' && line[i] != '<' && line[i] != '>')
		i++;
	end = i;
	value2 = substrdup(start, end, line);
	*fullvalue = ft_strjoin(value, value2);
	free(value);
	free(value2);
	return (i);
}
