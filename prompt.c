/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/04/05 17:44:06 by ilhannou         ###   ########.fr       */
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

void	parsing(char *line)
{
	t_line	*parsed_line;
	int		i;

	parsed_line = malloc(sizeof(t_line));
	parsed_line->cmd = NULL;
	parsed_line->option = NULL;
	parsed_line->path = NULL;
	if (!parsed_line)
	{
		free(parsed_line);
		return ;
	}
	if (ft_strchr(line, ' '))
		parsed_line->full_line = ft_split(line, ' ');
	else
	{
		parsed_line->full_line = malloc(sizeof(char *) * 2);
		if (!parsed_line->full_line)
		{
			free(parsed_line);
			return ;
		}
		parsed_line->full_line[0] = ft_strdup(line);
		parsed_line->full_line[1] = NULL;
	}
	i = 0;
	while (parsed_line->full_line[i])
	{
		if (i == 0)
			parsed_line->cmd = parsed_line->full_line[0];
		else if (i == 1)
			parsed_line->option = parsed_line->full_line[1];
		else if (i == 2)
			parsed_line->path = parsed_line->full_line[2];
		i++;
	}
	printf("%s %s %s\n", parsed_line->cmd, parsed_line->option, parsed_line->path);

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
