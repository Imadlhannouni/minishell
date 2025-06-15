/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:36:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/15 16:54:21 by ilhannou         ###   ########.fr       */
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
		ft_free_split(split, 2);
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

void	replace_env_variables(t_token *tokens, t_env *env)
{
	t_token	*curr;
	t_env	*curr_env;
	char	*key;
	char	*chunk;
	char	*result;
	char	*tmp;
	int		i;
	int		start;

	while (tokens)
	{
		if (tokens->value && tokens->type != TOKEN_SIMPLE_QUOTE && strchr(tokens->value, '$'))
		{
			i = 0;
			result = ft_strdup("");
			while (tokens->value[i])
			{
				if (tokens->value[i] == '$')
				{
					i++;
					start = i;
					while (ft_isalnum(tokens->value[i]) || tokens->value[i] == '_')
						i++;
					key = substrdup(start, i, tokens->value);
					curr_env = env;
					while (curr_env)
					{
						if (ft_strcmp(curr_env->key, key) == 0)
							break;
						curr_env = curr_env->next;
					}
					if (curr_env)
					{
						tmp = result;
						result = ft_strjoin(tmp, curr_env->value);
						free(tmp);
					}
					free(key);
				}
				else
				{
					start = i;
					while (tokens->value[i] && tokens->value[i] != '$')
						i++;
					chunk = substrdup(start, i, tokens->value);
					tmp = result;
					result = ft_strjoin(tmp, chunk);
					free(tmp);
					free(chunk);
				}
			}
			free(tokens->value);
			tokens->value = result;
		}
		tokens = tokens->next;
	}
}
