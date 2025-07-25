/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:36:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/23 22:31:21 by ilhannou         ###   ########.fr       */
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
			free(env_splited);
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}

int	append_env_or_chunk(char *str, int i, char **clone_envi,
		char **result)
{
	int		start;
	char	*tmp;
	char	*key;
	char	*chunk;
	char	*env_val;

	if (str[i] == '$')
	{
		i++;
		start = i;
		while (ft_isalnum(str[i]) || str[i] == '_')
			i++;
		key = substrdup(start, i, str);
		env_val = get_env_value(key, clone_envi);
		tmp = *result;
		*result = ft_strjoin(tmp, env_val);
		free(tmp);
		free(key);
		free(env_val);
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
	int		j;
	char	**splited;

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
			if (ft_strchr(expanded, ' '))
			{
				splited = ft_split(expanded, ' ');
				free(expanded);
				expanded = ft_strdup("");
				j = 0;
				while (splited[j])
				{
					expanded = ft_strjoin(expanded, splited[j]);
					if (splited[j + 1] != NULL)
						expanded = ft_strjoin(expanded, " ");
					j++;
				}
			}
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
}
