/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:31 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/05 19:55:59 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void help_exec(t_exe *var, t_help *help)
{
	help->child = 1;
	if (var->is_ambiguis)
	{
		reset_redirections(help->std_fd);
		putstr_fd("Minishell : ambiguous redirect\n", 2);
		exit_free(1);
	}
	signal(SIGINT, help->prev_handler_int);
	signal(SIGQUIT, help->prev_handler_quit);
	if (handle_redirections(var) < 0)
	{
		close(help->std_fd[0]);
		close(help->std_fd[1]);		
		exit_free(1);
	}
}

void	check_path(char *path, int *e_code)
{
	char *str;
	if (*e_code == 1)
	{
		str = join_strings(path, " : ", "is a directory\n");
		putstr_fd(str, 2);
		*e_code = 126;
		return ;
	}
	if (*e_code == 2)
	{
		*e_code = 127;
		putstr_fd("Minishell: No such file or directory\n", 2);
		return ;	
	}
	if (!path)
		putstr_fd("Minishell : command not found\n", 2);
	else if (access(path, X_OK))
		perror("Minishell");
}

int	exec_command(t_exe *var, char **env, t_help *help)
{
	int		pid;
	int		e_code;
	int		status;
	char 	*path;

	status = -1;
	path = NULL;
	e_code = retrieve_path(var->arr[0], env, &path);
	if (e_code > 0)
		return (check_path(path, &e_code), e_code);
	pid = fork();
	if (pid < 0)
		return (perror("Minishell"), 1);
	if (pid == 0)
	{
		help_exec(var, help);
		execve(path, var->arr, env);
		perror("Minishell");
		reset_redirections(help->std_fd);
		exit_free(127);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sighandler);
	return (extract_status(status));
}

int helper_built_in(t_exe *var, char ***env, t_help *help)
{
	if (handle_redirections(var) < 0)
		return (1);
	if (var->arr && var->arr[0])
		return (exec_builtin(var, env, help));
	return 0;
}

int	execute(t_pipe *pipes, char ***env, t_help *help)
{
	t_exe	*var;
	int fd[2];
	int status;

	var = NULL;
	status = 0;	 
	group_pipes(pipes, &var);
	if (count_pipes(pipes) > 1)
		status = exec_pipe(var, env, count_pipes(pipes), help);
	else if (count_pipes(pipes) == 1)
	{
		if (dup_std(fd, help) < 0)
			return 1;
		if (!is_builtin(var->arr[0]))
			status = exec_command(var, *env, help);
		else
			status = helper_built_in(var, env, help);
		if (reset_redirections(fd) < 0)
			return 1;
	}
	return status;
}
