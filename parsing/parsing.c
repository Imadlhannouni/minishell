/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:50:19 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/26 17:46:09 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_redirection(int i, char *line, int *flag)
{
	if (line[i] == '>' && line[i + 1] != '>')
	{
		*flag = 4;
		return (i + 1);
	}
	else if (line[i] == '<' && line[i + 1] != '<')
	{
		*flag = 3;
		return (i + 1);
	}
	else if (line[i] == '>' && line[i + 1] == '>')
	{
		*flag = 2;
		return (i + 2);
	}
	else if (line[i] == '<' && line[i + 1] == '<')
	{
		*flag = 1;
		return (i + 2);
	}
	return (i);
}

static int	handle_new_command(t_token **tokens, int i, char *line,
		int *new_command, int *flag)
{
	i = handle_redirection(i, line, flag);
	if (line[i] != '>' && line[i] != '<' && line[i] != '|')
	{
		i = is_cmds_var(tokens, i, line, flag);
		*new_command = 0;
	}
	else
		i++;
	return (i);
}

static int	handle_token_cases(t_token **tokens, int i, char *line,
		int *new_command, int *flag)
{
	if (line[i] == '\'')
		i = is_simple_quote(*tokens, i, line, flag);
	else if (line[i] == '"')
		i = is_double_quote(*tokens, i, line, flag);
	else if (line[i] == '<' || line[i] == '>')
		i = is_directions(i, line, flag);
	else if (line[i] == '|')
	{
		i = is_pipe(*tokens, i, line);
		*new_command = 1;
	}
	else if (line[i] == '-' && line[i + 1] == 'n' && (line[i + 2] == ' '
			|| line[i + 2] == '\t' || line[i + 2] == '\0'))
		i = is_option(*tokens, i, line);
	else
	{
		if (line[i] == '\0')
			return (i);
		i = is_word(*tokens, i, line, flag);
	}
	return (i);
}

t_token	*smart_split(char *line)
{
	t_token	*tokens;
	int		i;
	int		new_command;
	int		flag;

	flag = 0;
	tokens = NULL;
	i = 0;
	new_command = 1;
	if (handle_errors(line))
		return (tokens);
	while (line[i] != '\0')
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (new_command)
			i = handle_new_command(&tokens, i, line, &new_command, &flag);
		else
			i = handle_token_cases(&tokens, i, line, &new_command, &flag);
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
		free(current_token->value);
		current_token->value = create_heredoc_file(content);
		current_token->heredoc = 1;
		free(content);
		return (1);
	}
	return (0);
}

int	handle_heredocs(t_pipe *pipe, char **clone_envi, char **exit_code)
{
	t_pipe	*current;
	t_token	*current_token;

	current = pipe;
	while (current)
	{
		current_token = current->full_cmd;
		while (current_token)
		{
			if (current_token->heredoc == 1)
				if (!handle_heredoc_token(current_token, clone_envi, exit_code))
					return (0);
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

	*pipes = NULL;
	tokens = smart_split(line);
	if (!tokens)
		return (0);
	*pipes = group_tokens_into_pipes(tokens);
	if (!handle_heredocs(*pipes, clone_envi, &exit_code))
		return (0);
	if (!replace_env_variables(*pipes, clone_envi, exit_code))
		return (0);
	is_path(*pipes);
	//print_pipes(*pipes);
	return (1);
}
