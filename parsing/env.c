/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:36:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/18 16:30:58 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(char **envp, t_env **env)
{
	int		i;
	char	**split;

	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], '=');
		if (split && split[0])
		{
			if (split[1])
				add_env(env, split[1], split[0]);
			else
				add_env(env, "", split[0]);
		}
		free_arr(split, 2);
		i++;
	}
	return (*env);
}

void	add_env(t_env **head, char *value, char *key)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->value = ft_strdup(value);
	new->key = ft_strdup(key);
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

static char	*get_env_value(const char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

static int	append_env_or_chunk(char *str, int i, t_env *env,
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
		*result = ft_strjoin(tmp, get_env_value(key, env));
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

void	replace_env_variables(t_token *tokens, t_env *env)
{
	char	*expanded;
	int		i;

	while (tokens)
	{
		if (tokens->value && tokens->type != TOKEN_SIMPLE_QUOTE
			&& strchr(tokens->value, '$'))
		{
			i = 0;
			expanded = ft_strdup("");
			while (tokens->value[i])
				i = append_env_or_chunk(tokens->value, i, env, &expanded);
			free(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
}
