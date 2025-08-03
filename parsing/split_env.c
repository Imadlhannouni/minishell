/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 21:59:11 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/03 13:56:12 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*allocate_word(const char *s, int start, int end)
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

static char	**allocate(const char *s, char c)
{
	char	**arr;
	int		i;

	i = 0;
	arr = (char **)ft_malloc(sizeof(char *) * 3, 0);
	while (s[i] && s[i] != c)
		i++;
	arr[0] = allocate_word(s, 0, i - 1);
	if (s[i] == c)
		arr[1] = allocate_word(s, i + 1, ft_strlen(s) - 1);
	else
		arr[1] = ft_strdup("");
	if (!arr[1])
	{
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
