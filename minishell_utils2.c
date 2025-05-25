/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:01:45 by ilhannou          #+#    #+#             */
/*   Updated: 2025/05/25 22:31:42 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
	{
		i++;
	}
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i] && i < n)
	{
		i++;
	}
	if (i < n)
		return ((unsigned char) s1[i] - (unsigned char) s2[i]);
	return (0);
}

int	character_count(char *s)
{
	int	i = 0;
	int	in_single = 0;
	int	in_double = 0;

	while (s[i])
	{
		if (s[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (s[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (!in_single && !in_double);
}

int	ft_isalpha(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!((s[i] >= 65 && s[i] <= 90) || (s[i] >= 97 && s[i] <= 122)))
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	len1;
	size_t	len2;
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc(len1 + len2 + 1);
	if (!res)
		return (NULL);
	while (i < len1)
	{
		res[i] = s1[i];
		i++;
	}
	while (i < len1 + len2)
	{
		res[i] = s2[i - len1];
		i++;
	}
	res[i] = '\0';
	return (res);
}
