/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:36:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/15 14:47:22 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static char	*get_env_value(const char *key, char **clone_envi)
{
	int		i;
	char	**env_splited;
	char	*value;

	i = 0;
	while (clone_envi[i] != NULL)
	{
		if (ft_strncmp(clone_envi[i], key, ft_strlen(key)) == 0 && clone_envi[i][ft_strlen(key)] == '=')
		{
			env_splited = ft_split_env(clone_envi[i], '=');
			value = ft_strdup(env_splited[1]);
			free(env_splited[0]);
			free(env_splited[1]);
			return (value);
		}
		i++;
	}
	return ("");
}

int	append_env_or_chunk(char *str, int i, char **clone_envi,
		char **result)
{
	int		start;
	char	*tmp;
	char	*key;
	char	*chunk;

	if (str[i] == '$')
	{
		i++;
		start = i;
		while (ft_isalnum(str[i]) || str[i] == '_')
			i++;
		key = substrdup(start, i, str);
		tmp = *result;
		*result = ft_strjoin(tmp, get_env_value(key, clone_envi));
		free(tmp);
		free(key);
	}
	else
	{
		start = i;
		while (str[i] && str[i] != '$')
			i++;
		chunk = substrdup(start, i, str);
		tmp = *result;
		*result = ft_strjoin(tmp, chunk);
		free(tmp);
		free(chunk);
	}
	return (i);
}

void	replace_env_variables(t_token *tokens, char **clone_envi)
{
	char	*expanded;
	int		i;

	while (tokens)
	{
		if (tokens->value && tokens->type != TOKEN_SIMPLE_QUOTE
			&& strchr(tokens->value, '$') && tokens->heredoc == 0)
		{
			i = 0;
			expanded = ft_strdup("");
			while (tokens->value[i])
				i = append_env_or_chunk(tokens->value, i, clone_envi, &expanded);
			free(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
}
