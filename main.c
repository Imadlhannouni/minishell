/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/22 13:57:11 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*readline_func(char *line)
{
	line = readline("minishell> ");
	if (!line)
		exit(0);
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
    t_env	*env;
    t_pipe	*pipes;

    (void)argc;
    (void)argv;
    env = NULL;
    init_env(envp, &env);
    signal(SIGINT, sighandler);
    signal(SIGQUIT, SIG_IGN);
    line = NULL;
    while (1)
    {
        line = readline_func(line);
        if (!line)
            break ;
        pipes = NULL;
        main_parsing(line, &env, &pipes);
		while (pipes)
		{
			if (pipes->full_cmd)
			{
				if (pipes->full_cmd->type == TOKEN_CMD)
				{
					if (pipes->full_cmd->value)
					{
						if (!(ft_strcmp(pipes->full_cmd->value, "echo") == 0 || ft_strcmp(pipes->full_cmd->value,
				"cd") == 0 || ft_strcmp(pipes->full_cmd->value, "ls") == 0
			|| ft_strcmp(pipes->full_cmd->value, "pwd") == 0 || ft_strcmp(pipes->full_cmd->value,
				"export") == 0 || ft_strcmp(pipes->full_cmd->value, "unset") == 0
			|| ft_strcmp(pipes->full_cmd->value, "env") == 0 || ft_strcmp(pipes->full_cmd->value,
				"exit") == 0)) // replace with our own command execution logic
						{
							print_cmd_not_found(pipes->full_cmd->value);
						}
					}
				}
			}
			pipes = pipes->nextpipe;
		}
        free(line);
        free_pipes(&pipes);
    }
	free_env(env);
    return (0);
}
