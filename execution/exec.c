/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 22:50:31 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/09 11:55:21 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	help_exec(t_exe *var, t_help *help)
{
	help->child = 1;
	signal(SIGINT, help->prev_handler_int);
	signal(SIGQUIT, help->prev_handler_quit);
	if (handle_redirections(var) < 0)
		exit_free(1);
}

void	check_path(char *path, int *e_code)
{
	char	*str;

	if (*e_code == 1)
	{
		str = join_strings(path, " : ", "is a directory\n");
		*e_code = 126;
		return (putstr_fd(str, 2));
	}
	if (*e_code == 2)
	{
		str = ft_strdup(path);
		if (str[ft_strlen(str) - 1] == '/')
			str[ft_strlen(str) - 1] = 0;
		if (!check_directory(str) && access(str, F_OK) == 0)
		{
			str = join_strings(str, " : ", "is not a directory\n");
			return (*e_code = 126, putstr_fd(str, 2));
		}
		*e_code = 127;
		return (putstr_fd("Minishell: No such file or directory\n", 2));
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
	char	*path;

	status = -1;
	path = NULL;
	pid = fork();
	if (pid < 0)
		return (perror("Minishell"), 1);
	if (pid == 0)
	{
		help_exec(var, help);
		e_code = retrieve_path(var->arr[0], env, &path);
		if (e_code > 0)
			return (check_path(path, &e_code), exit_free(e_code), 0);
		execve(path, var->arr, env);
		perror("Minishell");
		exit_free(127);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, sighandler);
	return (extract_status(status));
}

int	helper_built_in(t_exe *var, char ***env, t_help *help)
{
	if (handle_redirections(var) < 0)
		return (1);
	if (var->arr && var->arr[0])
		return (exec_builtin(var, env, help));
	return (0);
}

int	execute(t_pipe *pipes, char ***env, t_help *help)
{
	t_exe	*var;
	int		status;

	var = NULL;
	status = 0;
	group_pipes(pipes, &var);
	if (count_pipes(pipes) > 1)
		status = exec_pipe(var, env, count_pipes(pipes), help);
	else if (count_pipes(pipes) == 1)
	{
		if (!is_builtin(var->arr[0]))
			status = exec_command(var, *env, help);
		else
		{
			if (dup_std(help) < 0)
				return (1);
			status = helper_built_in(var, env, help);
			if (reset_redirections(help->std_fd) < 0)
				return (1);
		}
	}
	return (status);
}
