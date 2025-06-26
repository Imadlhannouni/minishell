/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:50:19 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/22 13:47:34 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_new_command(t_token **tokens, int i, char *line,
		int *new_command)
{
	t_token_type	type;

	if (line[i] != '>' && line[i] != '<' && line[i] != '|')
	{
		i = is_cmds_var(tokens, type, i, line);
		*new_command = 0;
	}
	return (i);
}

static int	handle_token_cases(t_token **tokens, int i, char *line,
		int *new_command)
{
	t_token_type	type;

	if (line[i] == '\'')
		i = is_simple_quote(*tokens, type, i, line);
	else if (line[i] == '"')
		i = is_double_quote(*tokens, type, i, line);
	else if (line[i] == '<' || line[i] == '>')
		i = is_directions(*tokens, type, i, line);
	else if (line[i] == '|')
	{
		i = is_pipe(*tokens, type, i, line);
		*new_command = 1;
	}
	else if (line[i] == '-' && line[i + 1] == 'n' && (line[i + 2] == ' '
			|| line[i + 2] == '\t' || line[i + 2] == '\0'))
		i = is_option(*tokens, type, i, line);
	else
	{
		if (line[i] == '\0')
			return (i);
		i = is_word(*tokens, type, i, line);
	}
	return (i);
}

t_token	*smart_split(char *line, t_pipe *pipe)
{
	t_token	*tokens;
	int		i;
	int		new_command;

	tokens = NULL;
	i = 0;
	new_command = 1;
	if (character_count(line) == 0)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (tokens);
	}
	while (line[i] != '\0')
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (new_command)
			i = handle_new_command(&tokens, i, line, &new_command);
		else
			i = handle_token_cases(&tokens, i, line, &new_command);
	}
	return (tokens);
}

void	main_parsing(char *line, t_env **env, t_pipe **pipes)
{
	t_token	*tokens;

	*pipes = NULL;
	tokens = smart_split(line, *pipes);
	if (!tokens)
		free_tokens(tokens);
	replace_env_variables(tokens, *env);
	*pipes = group_tokens_into_pipes(tokens);
	is_path(*pipes);
	print_pipes(*pipes);
	return ;
}
