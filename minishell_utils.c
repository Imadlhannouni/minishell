/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:41:38 by ilhannou          #+#    #+#             */
/*   Updated: 2025/04/05 17:21:59 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
		{
			return ((char *)(s + i));
		}
		i++;
	}
	if (s[i] == (char)c)
	{
		return ((char *)(s + i));
	}
	return (NULL);
}

size_t	ft_strcpy(char *dst, const char *src)
{
	size_t	i;
	size_t	l;

	l = 0;
	i = 0;
	while (src[l] != '\0')
		l++;
	dst = malloc(l * sizeof(char));
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (l);
}

char	*ft_strdup(const char *s1)
{
	size_t	len1;
	char	*s2;
	size_t	i;

	i = 0;
	len1 = ft_strlen(s1) + 1;
	s2 = malloc(len1);
	if (!s2)
		return (NULL);
	while (i < len1 - 1)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
