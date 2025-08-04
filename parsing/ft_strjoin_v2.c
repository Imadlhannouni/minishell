/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_v2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:51:28 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/03 13:51:29 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*allocate(size_t size)
{
	size_t	i;
	char	*full;

	i = 0;
	full = (char *)ft_malloc(size * sizeof(char) + 1, 0);
	while (i < size)
	{
		full[i] = 0;
		i++;
	}
	return (full);
}

char	*ft_strjoin_v2(char *s1, char *s2, int flag)
{
	size_t	i;
	char	*str;

	(void)flag;
	if (!s1 || !s2)
		return (NULL);
	str = allocate(ft_strlen(s1) + ft_strlen(s2));
	i = 0;
	while (i < ft_strlen(s1))
	{
		str[i] = s1[i];
		i++;
	}
	while (i < (ft_strlen(s1) + ft_strlen(s2)))
	{
		str[i] = s2[i - ft_strlen(s1)];
		i++;
	}
	str[i] = '\0';
	return (str);
}
