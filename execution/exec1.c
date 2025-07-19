#include "../minishell.h"

int	count_args(t_token *tok)
{
	t_token *temp;
	int cpt;

	if (!tok)
		return 0;
	temp = tok;
	cpt = 0;

	while (temp != NULL)
	{
		if ((temp->inp_red == 0) && (temp->heredoc == 0)
			&& (temp->out_red == 0) && (temp->out_app == 0))
			cpt++;
		temp = temp->next;
	}
	return cpt;
}

int	init_var(t_vars *var, size_t pipe_num)
{
	size_t j;

	j = 0;
	var->i = 0;
	var->pipe_num = pipe_num;
	if (pipe_num > 1)
	{
		var->pid = malloc((pipe_num) * sizeof(__pid_t));
		if (!var->pid)
			return 0;
		var->fd = malloc((pipe_num - 1) * sizeof(int[2]));
		if (!var->fd)
			return (free(var->pid), 0);
		while (j < var->pipe_num - 1)
		{
			if (pipe((var->fd)[j]) == -1)
				return (close_all(var->fd, j - 1),free(var->fd),free(var->pid),0);
			j++;
		}
	}
	return 1;
}


int helper(t_exe *tmp ,char ***env, char ***no_val, t_vars var)
{
	char *path;

	if (!is_builtin(tmp->arr[0]))
		path = retrieve_path(tmp->arr[0],*env);
	int pid = fork();
	if (pid < 0)
	{
		exit(1);
	}
	if (pid == 0)
	{
		close_fd(var.fd, var.i, var.pipe_num);
		switch_fd(var.fd, var.i, var.pipe_num - 1);
		if (!is_builtin(tmp->arr[0]))
		{
			if (execve(path, tmp->arr, *env) == -1)
				exit(EXIT_FAILURE);
		}
		exec_builtin(tmp->arr, env,no_val);
		exit(0);
	}
	free(path);
	return pid;
}

void exec_pipe(t_exe *grp, char ***envp, char ***no_val, size_t pipe_num)
{
	t_vars var;
	t_exe *tmp;


	if (!init_var(&var, pipe_num))
	{
		return;
	}
	tmp = grp;
	while (var.i < pipe_num) 
	{
		var.pid[var.i++] = helper(tmp, envp, no_val, var);
		tmp = tmp->next;
	}
	var.i = 0;
	while(var.i < var.pipe_num - 1)	
	{
		close(var.fd[var.i][0]);
		close(var.fd[var.i++][1]);
	}
	var.i = 0;
	waitpid(var.pid[var.pipe_num - 1],NULL,0);
	while (var.i < var.pipe_num - 1)
	{
		wait(NULL);
		var.i++;
	}
	free(var.pid);
	free(var.fd);
}
