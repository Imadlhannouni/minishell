/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 21:58:55 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/10 22:02:16 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static char	**allocate(const char *s, char c)
{
	char	**arr;
	int		i;

	i = 0;
	arr = malloc(sizeof(char *) * 3);
	if (!arr)
		return (NULL);
	while (s[i] && s[i] != c)
		i++;
	arr[0] = allocate_word(s, 0, i - 1);
	if (!arr[0])
		return (free(arr), NULL);
	if (s[i] == c)
		arr[1] = allocate_word(s, i + 1, ft_strlen(s) - 1);
	else
		arr[1] = ft_strdup("");
	if (!arr[1])
	{
		free(arr[0]);
		free(arr);
		return (NULL);
	}
	arr[2] = NULL;
	return (arr);
}

char	**ft_split_env(const char *s, char c)
{
	if (!s)
		return (NULL);
	return (allocate(s, c));
}