/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/26 15:51:47 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*readline_func(char ***clone_envi)
{
	char	*line;

	line = readline("minishell> ");
	if (!line)
	{
		free_2d_arr(*clone_envi);
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
    char		*line;
    t_pipe		*pipes;
	char		**clone_envi;
	static char	*exit_code;
	int 		s;

	clone_envi = clone_env(envp);
    (void)argc;
    (void)argv;
    signal(SIGINT, sighandler);
    signal(SIGQUIT, SIG_IGN);
    line = NULL;
	exit_code = ft_strdup("0");
    while (1)
    {
        line = readline_func(&clone_envi);
        if (!line)
            break ;
        pipes = NULL;
		if (main_parsing(line, clone_envi, &pipes, exit_code))
		{
			s = execute(pipes, &clone_envi);
			if (exit_code[0] != '0')
				free(exit_code);
			exit_code = ft_itoa(s);
		}
        free(line);
        cleanup_heredoc_files(pipes);
        free_pipes(&pipes);
    }
	free_2d_arr(clone_envi);
    return (0);
}
