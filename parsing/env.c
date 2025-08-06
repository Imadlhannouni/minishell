/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:36:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/06 22:46:14 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	append_env_or_chunk(char *str, int i, t_expand_ctx *ctx, char **result)
{
	if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i
				+ 1] == '?'))
		return (handle_env_var(str, i, ctx, result));
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
		tokens->ambigious = 1;
		return (0);
	}
	return (1);
}

static void	expand_split_or_assign(t_token *tokens, char *expanded)
{
	if ((ft_strchr(expanded, ' ') || ft_strchr(expanded, '\t')
			|| ft_strchr(expanded, '\n')) && tokens->type != TOKEN_DOUBLE_QUOTE)
	{
		handle_token_split(tokens, expanded);
	}
	else
	{
		if (ft_strlen(expanded) == 1 && tokens->type != TOKEN_DOUBLE_QUOTE
			&& tokens->type != TOKEN_SIMPLE_QUOTE && tokens->next
			&& (tokens->next->type == TOKEN_DOUBLE_QUOTE
				|| tokens->next->type == TOKEN_SIMPLE_QUOTE)
			&& tokens->is_fullstring == 1)
		{
			tokens->value = ft_strdup("");
		}
		else if (expanded[0] == '\0')
			tokens->value = NULL;
		else
			tokens->value = expanded;
	}
}

static int	expand_token_value(t_token *tokens, char **clone_envi,
		char **exit_code)
{
	int				i;
	char			*expanded;
	t_expand_ctx	ctx;

	ctx.clone_envi = clone_envi;
	ctx.exit_code = *exit_code;
	i = 0;
	expanded = ft_strdup("");
	while (tokens->value[i])
		i = append_env_or_chunk(tokens->value, i, &ctx, &expanded);
	if (ft_strchr(tokens->value, '$') && ft_strchr(tokens->value, '='))
		tokens->type = TOKEN_DOUBLE_QUOTE;
	if (ft_strchr(expanded, '$'))
		tokens->type = TOKEN_DOUBLE_QUOTE;
	if (!handle_ambiguous_redirect(tokens, expanded))
		return (0);
	expand_split_or_assign(tokens, expanded);
	return (1);
}

int	replace_env_variables(t_pipe **pipes, char **clone_envi, char **exit_code)
{
	t_pipe	*current;
	t_token	*tokens;

	current = *pipes;
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
					break ;
				}
			}
			tokens = tokens->next;
		}
		current = current->nextpipe;
	}
	return (1);
}
