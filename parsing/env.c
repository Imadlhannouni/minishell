/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:36:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/04 12:42:44 by ilhannou         ###   ########.fr       */
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

static int	handle_env_var(char *str, int i, char **clone_envi, char **result,
		char *exit_code)
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
		env_val = ft_strdup(exit_code);
	else
		env_val = get_env_value(key, clone_envi);
	tmp = *result;
	*result = ft_strjoin(tmp, env_val);
	return (i);
}

static int	handle_chunk(char *str, int i, char **result)
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

int	append_env_or_chunk(char *str, int i, char **clone_envi, char **result,
		char *exit_code)
{
	if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i
			+ 1] == '?'))
		return (handle_env_var(str, i, clone_envi, result, exit_code));
	else
		return (handle_chunk(str, i, result));
}

static int	handle_ambiguous_redirect(t_token *tokens, char *expanded)
{
	if ((ft_strchr(expanded, ' ') || ft_strchr(expanded, '\t')
			|| ft_strchr(expanded, '\n') || ft_strlen(expanded) == 0)
		&& tokens->type != TOKEN_DOUBLE_QUOTE && (tokens->out_app
			|| tokens->out_red || tokens->inp_red))
	{
		return (0);
	}
	return (1);
}

static void	handle_token_split(t_token *tokens, char *expanded)
{
	char			**splited;
	t_token			*tmp;
	t_token_type	type;
	int				i;

	splited = ft_split2(expanded, ' ');
	tmp = tokens->next;
	type = tokens->type;
	tokens->value = ft_strdup(splited[0]);
	tokens->next = NULL;
	for (i = 1; splited[i]; i++)
		add_token(&tokens, splited[i], type, 0);
	while (tokens->next)
		tokens = tokens->next;
	tokens->next = tmp;
}

static int	expand_token_value(t_token *tokens, char **clone_envi,
		char **exit_code)
{
	int		i;
	char	*expanded;

	i = 0;
	expanded = ft_strdup("");
	while (tokens->value[i])
		i = append_env_or_chunk(tokens->value, i, clone_envi, &expanded,
				*exit_code);
	if (ft_strchr(tokens->value, '$') && ft_strchr(tokens->value, '='))
		tokens->type = TOKEN_DOUBLE_QUOTE;
	if (ft_strchr(expanded, '$'))
		tokens->type = TOKEN_DOUBLE_QUOTE;
	if (!handle_ambiguous_redirect(tokens, expanded))
		return (0);
	if ((ft_strchr(expanded, ' ') || ft_strchr(expanded, '\t')
			|| ft_strchr(expanded, '\n')) && tokens->type != TOKEN_DOUBLE_QUOTE)
		handle_token_split(tokens, expanded);
	else
	{
		if (ft_strlen(expanded) == 1 && tokens->type != TOKEN_DOUBLE_QUOTE
			&& tokens->type != TOKEN_SIMPLE_QUOTE && tokens->next
			&& (tokens->next->type == TOKEN_DOUBLE_QUOTE
				|| tokens->next->type == TOKEN_SIMPLE_QUOTE)
			&& tokens->is_fullstring == 1)
			tokens->value = ft_strdup("");
		else if (expanded[0] == '\0')
			tokens->value = NULL;
		else
			tokens->value = expanded;
	}
	return (1);
}

int	replace_env_variables(t_pipe **pipes, char **clone_envi, char **exit_code)
{
	t_pipe	*current;
	t_pipe	*prev;
	t_token	*tokens;

	current = *pipes;
	prev = NULL;
	while (current)
	{
		tokens = current->full_cmd;
		while (tokens)
		{
			if (tokens->value && tokens->type != TOKEN_SIMPLE_QUOTE
				&& ft_strchr(tokens->value, '$') && tokens->heredoc == 0
				&& tokens->expand == 0)
			{
				if (!expand_token_value(tokens, clone_envi, exit_code))
				{
					current->ambigious = 1;
					break ;
				}
			}
			tokens = tokens->next;
		}
		current = current->nextpipe;
	}
	return (1);
}
