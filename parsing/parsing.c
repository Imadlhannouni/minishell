/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:50:19 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/07 21:10:05 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_token_cases(t_token **tokens, int i, char *line, int *flag)
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

t_token	*smart_split(char *line, char **exit_code)
{
	t_token	*tokens;
	int		i;
	int		flag;

	flag = 0;
	tokens = NULL;
	i = 0;
	if (handle_errors(line, exit_code))
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

int	handle_heredoc_token(t_token *current_token, char **clone_envi,
		char **exit_code)
{
	char	*filename;

	filename = read_heredoc(current_token->value, clone_envi,
			current_token->type, exit_code);
	if (filename)
	{
		current_token->value = filename;
		current_token->heredoc = 1;
		return (1);
	}
	return (0);
}

int	handle_heredocs(t_pipe *pipe, char **clone_envi, char **exit_code)
{
	t_pipe	*current;

	current = pipe;
	while (current)
	{
		if (!handle_heredoc_tokens(current->full_cmd, clone_envi, exit_code))
			return (0);
		if (current->nextpipe == NULL)
			break ;
		current = current->nextpipe;
	}
	return (1);
}

void	print_pipes(t_pipe *pipes)
{
	t_pipe	*curr_pipe;
	t_token	*curr_token;

	curr_pipe = pipes;
	while (curr_pipe)
	{
		printf("=== New Pipe Command ===\n");
		curr_token = curr_pipe->full_cmd;
		while (curr_token)
		{
			printf("%s : %d : %d | ambigious : %d, inp_red : %d, out_red : %d,  heredoc : %d, out_app : %d\n", curr_token->value, curr_token->type,
				curr_token->is_fullstring, curr_token->ambigious ,curr_token->inp_red,
				curr_token->out_red, curr_token->heredoc, curr_token->out_app);
			curr_token = curr_token->next;
		}
		curr_pipe = curr_pipe->nextpipe;
	}
}

int	main_parsing(char *line, char **clone_envi, t_pipe **pipes,
		char **exit_code)
{
	t_token	*tokens;
	t_pipe	*curr;

	tokens = smart_split(line, exit_code);
	free(line);
	if (!tokens)
		return (0);
	*pipes = group_tokens_into_pipes(tokens);
	if (!handle_heredocs(*pipes, clone_envi, exit_code))
		return (0);
	if (!replace_env_variables(pipes, clone_envi, exit_code))
		return (0);
	print_pipes(*pipes);
	curr = *pipes;
	while (curr)
	{
		compact_fullstrings(&(curr)->full_cmd);
		curr = curr->nextpipe;
	}
	return (1);
}
