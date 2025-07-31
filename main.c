/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/31 16:25:09 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*readline_func(char ***clone_envi, char **exit_code)
{
	char	*line;

	line = readline("minishell> ");
	if (!line)
	{
		free(*exit_code);
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
static void	init_collect(t_free *collect, char **exit_code)
{
	collect->env = NULL;
	collect->exe = NULL;
	collect->fd = NULL;
	collect->no_val = NULL;
	collect->pid = NULL;
	collect->pipes = NULL;
	collect->fds = NULL;
	collect->path = NULL;
	(collect->exit_code) = exit_code;
}

int	main(int argc, char **argv, char **envp)
{
    char	*line;
    t_pipe	*pipes;
	t_free	collect;
	char	**clone_envi;
	static char *exit_code;
	int s;

	clone_envi = clone_env(envp);
    (void)argc;
    (void)argv;
    line = NULL;
	exit_code = NULL;
	init_collect(&collect, &exit_code);
    collect.prev_handler_int = signal(SIGINT, sighandler);
    collect.prev_handler_quit = signal(SIGQUIT, SIG_IGN);
	exit_code = ft_strdup("0");
    while (1)
	{
		line = readline_func(&clone_envi, &exit_code);
		if (!line)
            break ;
        pipes = NULL;
		
		if (main_parsing(line, clone_envi, &pipes, exit_code))
		{
			s = execute(pipes, &clone_envi, &collect);
			// printf("exit code = %d \n",s);
			free(exit_code);
			exit_code = ft_itoa(s);
		}
        free(line);
		cleanup_heredoc_files(pipes);
        free_pipes(&pipes);
    }
	free(exit_code);
	free_2d_arr(clone_envi);
    return (0);
}
