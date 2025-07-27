/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/26 15:07:52 by abbenmou         ###   ########.fr       */
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
    char	*line;
    t_pipe	*pipes;
	char	**clone_envi;
	int s = 0;

	clone_envi = clone_env(envp);
    (void)argc;
    (void)argv;
    signal(SIGINT, sighandler);
    signal(SIGQUIT, SIG_IGN);
    line = NULL;
    while (1)
    {
        line = readline_func(&clone_envi);
        if (!line)
            break ;
        pipes = NULL;
		if (main_parsing(line, clone_envi, &pipes))
			s = execute(pipes, &clone_envi);
		printf("exit_code = %d\n", s);
        free(line);
        cleanup_heredoc_files(pipes);
        free_pipes(&pipes);
    }
	free_2d_arr(clone_envi);
    return (0);
}
