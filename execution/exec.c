#include "../minishell.h"

void exec_command(t_exe *var, char **env)
{
	int pid;
	char *path = retrieve_path(var->arr[0],env);
	if (!path)
	 	return;
	pid = fork();
	if (pid == 0)
	{
		handle_redirections(var, NULL);
		execve(path, var->arr, env);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
	free(path);
}

void group_pipes(t_pipe *pipes, t_exe **var)
{
	t_pipe *tmp;

	tmp = pipes;
	while (tmp)
	{
		add_node(var,creat_node(tmp->full_cmd));
		tmp = tmp->nextpipe;
	}
}

void	execute(t_pipe *pipes, char ***env)
{
	t_exe	*var = NULL;
	static char **no_val = NULL;
	int fd[2];

	fd[0] = -1;
	fd[1] = -1;
	group_pipes(pipes, &var);
	if (count_pipes(pipes) > 1)
		exec_pipe(var, env, &no_val, count_pipes(pipes));
	else if (count_pipes(pipes) == 1)
	{
		if (!is_builtin(var->arr[0]))
			exec_command(var, *env);
		else if(is_builtin(var->arr[0]))
		{
			handle_redirections(var, fd);
			exec_builtin(var, env, &no_val);
			if (fd[0] >= 0 || fd[1] >= 0)
			{
				if (var->out_red_type)
					dup2(fd[1] ,STDOUT_FILENO);
				if (var->in_red_type)
					dup2(fd[0] ,STDIN_FILENO);
			}
		}
	}
	free_t_exe(&var);
}
