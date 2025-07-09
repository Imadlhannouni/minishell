/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/26 14:26:08 by ilhannou         ###   ########.fr       */
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
    // t_env	*env;
	char **env_clone;
	t_pipe	*pipes;

    (void)argc;
    (void)argv;
    //env = NULL;
    // init_env(envp, &env);
    
	env_clone = clone_env(envp);
	
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
		execute(pipes,env_clone);
        // free(line);
        // free_pipes(&pipes);
    }
	free_2d_arr(env_clone);
    return (0);
}
