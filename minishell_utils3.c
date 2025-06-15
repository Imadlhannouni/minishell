/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:45:00 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/15 16:45:07 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*substrdup(int start, int end, char	*str)
{
	char	*new;
	int		i;
	
	new = malloc(end - start + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (start < end)
		new[i++] = str[start++];
	new[i] = '\0';
	return (new);
}
