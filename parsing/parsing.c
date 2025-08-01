/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:50:19 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/01 10:59:57 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_token_cases(t_token **tokens, int i, char *line,
		int *flag)
{
	if (line[i] == '\'')
		i = is_simple_quote(tokens, i, line, flag);
	else if (line[i] == '"')
		i = is_double_quote(tokens, i, line, flag);
	else if (line[i] == '<' || line[i] == '>')
		i = is_directions(i, line, flag);
	else if (line[i] == '|')
	{
		i = is_pipe(*tokens, i, line);
	}
	else
	{
		if (line[i] == '\0')
			return (i);
		i = is_word(tokens, i, line, flag);
	}
	return (i);
}

t_token	*smart_split(char *line)
{
	t_token	*tokens;
	int		i;
	int		flag;

	flag = 0;
	tokens = NULL;
	i = 0;
	if (handle_errors(line))
		return (tokens);
	while (line[i] != '\0')
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		i = handle_token_cases(&tokens, i, line, &flag);
	}
	return (tokens);
}

static int	handle_heredoc_token(t_token *current_token, char **clone_envi,
		char **exit_code)
{
	char	*content;

	content = read_heredoc(current_token->value, clone_envi,
			current_token->type, exit_code);
	if (content)
	{
		current_token->value = create_heredoc_file(content);
		current_token->heredoc = 1;
		return (1);
	}
	return (0);
}

int	handle_heredocs(t_pipe *pipe, char **clone_envi, char **exit_code)
{
	t_pipe	*current;
	t_token	*current_token;
	t_token	*compact;

	current = pipe;
	while (current)
	{
		current_token = current->full_cmd;
		while (current_token)
		{
			if (current_token->heredoc == 1)
			{
				compact = current_token;
				if (current_token->is_fullstring == 1)
				{
					concat_fullstring(compact, NULL);
					current_token->is_fullstring = 0;
				}
				if (!handle_heredoc_token(current_token, clone_envi, exit_code))
					return (0);
			}
			current_token = current_token->next;
		}
		if (current->nextpipe == NULL)
			break ;
		current = current->nextpipe;
	}
	return (1);
}

int	main_parsing(char *line, char **clone_envi, t_pipe **pipes, char *exit_code)
{
	t_token	*tokens;
	t_pipe	*curr;

	*pipes = NULL;
	tokens = smart_split(line);
	if (!tokens)
		return (0);
	*pipes = group_tokens_into_pipes(tokens);
	if (!handle_heredocs(*pipes, clone_envi, &exit_code))
		return (0);
	if (!replace_env_variables(pipes, clone_envi, exit_code))
		return (0);
	curr = *pipes;
	while (curr)
	{
		compact_fullstrings(&(curr)->full_cmd);
		curr = curr->nextpipe;
	}
	return (1);
}
