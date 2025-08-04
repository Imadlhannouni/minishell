/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:44:24 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/04 17:05:10 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	global_var2(int new_value)
{
	static int	value = 0;

	if (new_value != -1)
		value = new_value;
	return (value);
}

char	*free_line(char *new_value, int i)
{
	static char	*value = NULL;

	if (i != -1)
	{
		if (value)
			free(value);
		value = new_value;
	}
	return (value);
}

void	expand_and_write_line(struct s_heredoc *heredoc, char *line,
		char **exit_code, char **clone_envi)
{
	char			*expanded_line;
	int				i;
	t_expand_ctx	ctx;

	ctx.clone_envi = clone_envi;
	ctx.exit_code = *exit_code;
	i = 0;
	if (heredoc->type != 9 && heredoc->type != 3 && heredoc->type != 4
		&& ft_strchr(line, '$'))
	{
		expanded_line = ft_strdup("");
		while (line[i])
			i = append_env_or_chunk(line, i, &ctx, &expanded_line);
		line = expanded_line;
	}
	write(heredoc->write_fd, line, ft_strlen(line));
	write(heredoc->write_fd, "\n", 1);
}

void	cleanup_heredoc_files(t_pipe *pipes)
{
	t_pipe	*current_pipe;
	t_token	*current_token;

	current_pipe = pipes;
	while (current_pipe)
	{
		current_token = current_pipe->full_cmd;
		while (current_token)
		{
			if (current_token->heredoc == 1 && current_token->value)
				unlink(current_token->value);
			current_token = current_token->next;
		}
		current_pipe = current_pipe->nextpipe;
	}
}
