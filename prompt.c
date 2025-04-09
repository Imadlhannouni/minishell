/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/04/09 18:24:15 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*readline_func(char *line)
{
	line = readline("minishell> ");
	if (!line)
		exit(1);
	else if (*line)
		add_history(line);
	return (line);
	//printf("%s\n", line);
}

void add_token(t_token **head, char *value, t_token_type type)
{
	t_token *new;
	t_token *tmp;

	new = malloc(sizeof(t_token));
	new->type = type;
	new->value = value;
	new->next = NULL;
	if (!*head)
		*head = new;
	else {
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void add_pipe(t_pipe **head, t_token *fullcmd)
{
	t_pipe *new;
	t_pipe *tmp;

	new = malloc(sizeof(t_pipe));
	if (!new)
		return;
	new->full_cmd = fullcmd;
	new->nextpipe = NULL;
	if (!*head)
		*head = new;
	else {
		tmp = *head;
		while (tmp->nextpipe)
			tmp = tmp->nextpipe;
		tmp->nextpipe = new;
	}
}

t_pipe	*group_tokens_into_pipes(t_token *tokens)
{
	t_pipe	*pipes;
	t_token	*start;
	t_token	*curr;
	t_token	*prev;

	pipes = NULL;
	start = tokens;
	curr = tokens;
	prev = NULL;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE)
		{
			curr->value = NULL;
			if (prev)
				prev->next = NULL;
			add_pipe(&pipes, start);
			start = curr->next;
		}
		prev = curr;
		curr = curr->next;
	}
	if (start)
		add_pipe(&pipes, start);
	return (pipes);
}

void print_pipes(t_pipe *pipes)
{
	t_pipe *curr_pipe = pipes;
	t_token *curr_token;

	while (curr_pipe)
	{
		printf("=== New Pipe Command ===\n");
		curr_token = curr_pipe->full_cmd;
		while (curr_token)
		{
			printf("%s : %d\n", curr_token->value, curr_token->type);
			curr_token = curr_token->next;
		}
		curr_pipe = curr_pipe->nextpipe;
	}
}


char	*substrdup(int start, int end, char	*str)
{
	char	*new;
	int		i;
	
	new = malloc(end - start + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (start < end)
		new[i++] = str[start++];
	new[i] = '\0';
	return (new);
}

t_token	*smart_split(char *line, t_pipe *pipe)
{
	t_token			*tokens;
	t_token_type	type;
	t_token			*last;
	int				i;
	int				new_command;

	tokens = NULL;
	i = 0;
	new_command = 1;
	while (line[i] != '\0')
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (new_command)
		{
			i = is_cmds_var(&tokens, type, i, line);
			new_command = 0;
		}
		else if (line[i] == '\'')
			i = is_simple_quote(tokens, type, i, line);
		else if (line[i] == '"')
			i = is_double_quote(tokens, type, i, line);
		else if (line[i] == '<' || line[i] == '>')
			i = is_directions(tokens, type, i, line);
		else if (line[i] == '|')
		{
			i = is_pipe(tokens, type, i, line);
			add_pipe(&pipe, tokens);
			new_command = 1;
		}
		else
			i = is_word(tokens, type, i, line);
	}
	return (tokens);
}
	

void	parsing(char *line)
{
	t_token	*tokens;
	t_pipe	*pipes;
	
	pipes = NULL;
	tokens = smart_split(line, pipes);
	pipes = group_tokens_into_pipes(tokens);
	print_pipes(pipes);
	return ;
}

int main()
{
    char *line;
	
	while (1)
	{
		line = readline_func(line);
		parsing(line);
	}
	return (0);
}
