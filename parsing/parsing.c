/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:50:19 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/14 14:02:16 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_new_command(t_token **tokens, int i, char *line,
		int *new_command, int *flag)
{
	if (line[i] == '<' && line[i + 1] == '<')
	{
		*flag = 1;
		i += 2;
	}
	if (line[i] != '>' && line[i] != '<' && line[i] != '|')
	{
		i = is_cmds_var(tokens, i, line, flag);
		*new_command = 0;
	}
	else
		i++; // error here
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
		i = is_directions(*tokens, i, line, flag);
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
			i = handle_new_command(&tokens, i, line, &new_command, &flag);
		else
			i = handle_token_cases(&tokens, i, line, &new_command, &flag);
	}
	return (tokens);
}

void	handle_heredocs(t_token *token)
{
    t_token	*current;
    char	*content;

    current = token;
    while (current)
    {
        if (current->heredoc == 1)
        {
            content = read_heredoc(current->value);
            if (content)
            {
                current->heredoc_filename = create_heredoc_file(content);
                free(content);
            }
        }
        current = current->next;
    }
}

void	main_parsing(char *line, char **clone_envi, t_pipe **pipes)
{
	t_token		*tokens;
	// t_token		*tmp;
	
	*pipes = NULL;
	tokens = smart_split(line);
	if (!tokens)
		return ;
	handle_heredocs(tokens);
	// tmp = tokens;
	// while (tmp)
	// {
	// 	if (tmp->heredoc == 1)
	// 	{
	// 		printf("%s", tmp->heredoc_content);
	// 	}
	// 	tmp = tmp->next;
	// }
	replace_env_variables(tokens, clone_envi);
	*pipes = group_tokens_into_pipes(tokens);
	is_path(*pipes);
	print_pipes(*pipes);
	return ;
}
