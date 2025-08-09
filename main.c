/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/08 21:39:16 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	global_var(int new_value)
{
	static int	value = 0;

	if (new_value != -1)
		value = new_value;
	return (value);
}

char	*readline_func(char ***clone_envi, char *exit_code)
{
	char	*line;
	int		exit_status;

	(void)clone_envi;
	line = readline("Minishell> ");
	if (!line)
	{
		exit_status = ft_atoi(exit_code);
		putstr_fd("exit\n", 2);
		ft_malloc(0, 1);
		free(line);
		exit(exit_status);
	}
	else if (*line)
		add_history(line);
	return (line);
}

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		signum = global_var(signum);
		putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	init_help(t_help *help, char **exit_code)
{
	help->prev_handler_quit = signal(SIGQUIT, SIG_IGN);
	help->prev_handler_int = signal(SIGINT, sighandler);
	help->exit_code = exit_code;
	help->child = 0;
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_pipe		*pipes;
	char		**clone_envi;
	static char	*exit_code;
	t_help		help;

	clone_envi = clone_env(envp);
	(void)argc;
	(void)argv;
	exit_code = ft_strdup("0");
	pipes = NULL;
	init_help(&help, &exit_code);
	while (1)
	{
		line = readline_func(&clone_envi, exit_code);
		if ((global_var(-1)) == SIGINT)
		{
			exit_code = ft_itoa(SIGINT + 128);
			global_var(0);
		}
		if (main_parsing(line, clone_envi, &pipes, &exit_code))
			exit_code = ft_itoa(execute(pipes, &clone_envi, &help));
		cleanup_heredoc_files(pipes);
	}
	return (0);
}
