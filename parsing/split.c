/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 21:58:55 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/01 11:05:53 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_word(char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*allocate_word(char *s, int start, int end)
{
	int		len;
	char	*word;
	int		i;

	len = end - start + 1;
	word = (char *)ft_malloc((len + 1) * sizeof(char), 0);
	i = 0;
	while (start <= end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

static char	**allocate(char *s, char c)
{
	char	**arr;
	int		i;
	int		j;
	int		start;

	i = -1;
	j = 0;
	start = -1;
	arr = (char **)ft_malloc((count_word(s, c) + 1) * sizeof(char *), 0);
	while (s[++i])
	{
		if (s[i] != c && start == -1)
			start = i;
		if ((s[i] != c && (s[i + 1] == c || s[i + 1] == '\0')))
		{
			arr[j++] = allocate_word(s, start, i);
			start = -1;
		}
	}
	arr[j] = NULL;
	return (arr);
}

char	**ft_split(const char *s, char c)
{
	if (!s)
		return (NULL);
	return (allocate((char *)s, c));
}