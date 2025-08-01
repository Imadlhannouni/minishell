/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/01 15:37:40 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*readline_func(char ***clone_envi)
{
	char	*line;
	(void)clone_envi;
	// if (isatty(STDIN_FILENO))
	// {
	line = readline("minishell> ");
	if (!line)
	{
		ft_malloc(0,1);
		exit(0);
	}
	else if (*line)
		add_history(line);
	// }
	// else
	// {
	// 	line = get_next_line(0);
	// 	if (line)
	// 		line[ft_strlen(line) - 1] = '\0';
	// }
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

void init_help(t_help *help, char **exit_code)
{
    help->prev_handler_quit = signal(SIGQUIT, SIG_IGN);
    help->prev_handler_int = signal(SIGINT, sighandler);
	help->exit_code = exit_code;
}

int	main(int argc, char **argv, char **envp)
{
    char	*line;
    t_pipe	*pipes;
	char	**clone_envi;
	static char *exit_code;
	t_help	help;
	int s;

	clone_envi = clone_env(envp);
    (void)argc;
    (void)argv;
    line = NULL;
	exit_code = ft_strdup("0");
	init_help(&help, &exit_code);
    while (1)
	{
		line = readline_func(&clone_envi);
		if (!line)
            break ;
		if (main_parsing(line, clone_envi, &pipes, &exit_code))
		{
			s = execute(pipes, &clone_envi, &help);
			exit_code = ft_itoa(s);
		}
		cleanup_heredoc_files(pipes);
    }
	ft_malloc(0,1);
    return (0);
}
