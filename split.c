/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:17:41 by ilhannou          #+#    #+#             */
/*   Updated: 2025/04/30 15:30:07 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count(const char *s, char c)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static char	*allocate_word(const char *s, int start, int end)
{
	int		len;
	char	*word;
	int		i;

	len = end - start + 1;
	word = (char *)malloc((len + 1) * sizeof(char));
	i = 0;
	if (!word)
		return (NULL);
	while (start <= end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

void	ft_free_split(char **split, int j)
{
	while (j > 0)
	{
		j--;
		free(split[j]);
	}
	free(split);
}

static char	**allocate(const char *s, char c)
{
	char	**arr;
	int		i;
	int		j;
	int		start;

	i = 0;
	j = 0;
	start = 0;
	arr = malloc((ft_count(s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			start = i;
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
		{
			arr[j++] = allocate_word(s, start, i);
			if (!arr[j - 1])
				return (ft_free_split(arr, j - 2), NULL);
		}
		i++;
	}
	arr[j] = NULL;
	return (arr);
}

char	**ft_split(const char *s, char c)
{
	if (!s)
		return (NULL);
	return (allocate(s, c));
}