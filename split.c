/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:27:58 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/18 16:21:04 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

 void	free_arr(char **arr, int j)
{
	while (j >= 0)
	{
		free(arr[j]);
		j--;
	}
	free(arr);
}

static char	*allocate_word(char *s, int start, int end)
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

static char	**allocate(char *s, char c)
{
	char	**arr;
	int		i;
	int		j;
	int		start;

	i = -1;
	j = 0;
	start = -1;
	arr = (char **)malloc((count_word(s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	while (s[++i])
	{
		if (s[i] != c && start == -1)
			start = i;
		if ((s[i] != c && (s[i + 1] == c || s[i + 1] == '\0')))
		{
			arr[j++] = allocate_word(s, start, i);
			if (!arr[j - 1])
				return (free_arr(arr, j - 2), NULL);
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
