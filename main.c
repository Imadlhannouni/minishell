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
	char **clone = clone_env(envp);
	char **no_val = NULL;
    while (1)
    {
        line = readline_func(line);
        if (!line)
            break ;
		if (strncmp(line, "cd", 2) == 0)
		{
			char *arg = line + 2;
			while (*arg == ' ' || *arg == '\t')
				arg++;
			if (*arg == '\0' || *arg == '\n')
				cd(NULL,&clone);
			else
			{
				cd(arg, &clone);
				char *old = retreive_var(clone,"OLDPWD");
				printf("%s\n",old);
			}
		}
		else if(strncmp(line, "pwd", 4) == 0)
			pwd();
		else if(strncmp(line, "env", 4) == 0)
			print_env(clone);
		else if (strncmp(line, "export", 6) == 0)
		{
			char *arg = line + 6;
			
			while (*arg == ' ' || *arg == '\t')
				arg++;
			if (*arg == '\0')
				export(&clone,NULL, &no_val);
			else
				export(&clone,arg, &no_val);
		}
		else if (strncmp(line, "unset", 5) == 0)
		{
			char *arg = line + 5;
			while (*arg == ' ' || *arg == '\t')
				arg++;
			unset(&clone, arg);
		}
		else if (strncmp(line, "exit", 4) == 0)
		{
			exit(0);
		}
        pipes = NULL;
        main_parsing(line, &env, &pipes);
        free(line);
        free_pipes(&pipes);
    }
	free_env(env);
    return (0);
}


