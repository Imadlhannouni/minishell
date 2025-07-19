/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/17 15:39:26 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*readline_func(char *line)
{
	line = readline("minishell> ");
	if (!line)
	{
		exit(0);
	}
	else if (*line)
		add_history(line);
	
	return (line);
}

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **envp)
{
    char	*line;
    t_pipe	*pipes;
	char	**clone_envi;

	clone_envi = clone_env(envp);
    (void)argc;
    (void)argv;
    signal(SIGINT, sighandler);
    signal(SIGQUIT, SIG_IGN);
    line = NULL;
    while (1)
    {
        line = readline_func(line);
        if (!line)
		{
            break ;
		}
		pipes = NULL;
		if (main_parsing(line, clone_envi, &pipes))
		{
			execute(pipes, &clone_envi);
		}
		free(line);
        free_pipes(pipes);
    }
    return (0);
}