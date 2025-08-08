/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 17:01:51 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/08 13:59:28 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(const char *key, char **clone_envi)
{
	int		i;
	char	**env_splited;
	char	*value;

	i = 0;
	while (clone_envi[i] != NULL)
	{
		if (ft_strncmp(clone_envi[i], key, ft_strlen(key)) == 0
			&& clone_envi[i][ft_strlen(key)] == '=')
		{
			env_splited = ft_split_env(clone_envi[i], '=');
			value = ft_strdup(env_splited[1]);
			return (value);
		}
		i++;
	}
	return (ft_strdup(""));
}

void	handle_token_split(t_token *tokens, char *expanded)
{
	char			**splited;
	t_token			*tmp;
	t_token_type	type;
	int				i;
	int				flag;

	splited = ft_split2(expanded, ' ');
	tmp = tokens->next;
	type = tokens->type;
	tokens->value = ft_strdup(splited[0]);
	tokens->next = NULL;
	i = 1;
	flag = 0;
	if (splited && *splited && tokens->is_fullstring == 1)
	{
		tokens->is_fullstring = 0;
		flag = 1;
	}
	while (splited && splited[i])
		add_token(&tokens, splited[i++], type, 0);
	while (tokens->next)
		tokens = tokens->next;
	if (flag == 1)
		tokens->is_fullstring = 1;
	tokens->next = tmp;
}

int	handle_chunk(char *str, int i, char **result)
{
	int		start;
	char	*tmp;
	char	*chunk;

	start = i;
	while (str[i] && (str[i] != '$' || !(ft_isalnum(str[i + 1]) || str[i
					+ 1] == '_' || str[i + 1] == '?')))
		i++;
	chunk = substrdup(start, i, str);
	tmp = *result;
	*result = ft_strjoin(tmp, chunk);
	return (i);
}

int	handle_env_var(char *str, int i, t_expand_ctx *ctx, char **result)
{
	int		start;
	char	*tmp;
	char	*key;
	char	*env_val;

	i++;
	start = i;
	if (str[i] == '?')
		i++;
	else
	{
		while (ft_isalnum(str[i]) || str[i] == '_')
			i++;
	}
	key = substrdup(start, i, str);
	if (ft_strcmp(key, "?") == 0)
		env_val = ft_strdup(ctx->exit_code);
	else
		env_val = get_env_value(key, ctx->clone_envi);
	tmp = *result;
	*result = ft_strjoin(tmp, env_val);
	return (i);
}
