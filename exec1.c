#include "minishell.h"

void close_fd(int **fd, int i, int total)
{
	int j = 0;

	if (i == 0)
	{
		close(fd[i][0]);
		j++;
	}
	else if (i == total - 1)
	{
		close(fd[i][1]);
		total--;
	}
	while (j < total)
	{
		if (j == i - 1)
			close(fd[j][1]);
		else if (j == i)
			close(fd[j][0]);
		else
		{
			close(fd[j][0]);
			close(fd[j][1]);
		}
		j++;
	}
}

void	switch_fd(int **fd, int i, int total)
{
	if (i == 0)
	{
		dup2(fd[0][1], STDOUT_FILENO);
		close(fd[0][1]);
	}
	else if (i == total - 1)
	{
		dup2(fd[i - 1][0], STDIN_FILENO);
		close(fd[i - 1][0]);
	}
	else
	{
		dup2(fd[i - 1][0], STDIN_FILENO);
		dup2(fd[i][1], STDOUT_FILENO);
		close(fd[i - 1][0]);
		close(fd[i][1]);
	}
}

int	var_num_v2(char ***arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		i++;
	}
	return i;
}

int	init_var(t_vars *var, char ***args)
{
	int j = 0;

	var->i = 0;
	var->pipe_num = var_num_v2(args);
	if (var->pipe_num > 1)
	{
		var->fd = malloc((var->pipe_num - 1) * sizeof(int*));
		if (!var->fd)
			return 0;
		var->pid = malloc((var->pipe_num) * sizeof(__pid_t));
		if (!var->pid)
			return 0;
		while (j < var->pipe_num)
		{
			var->fd[j] = malloc(2 * sizeof(int));
			if (!(var->fd)[j])
				return 0;
			if (pipe((var->fd)[j]) == -1)
				return 0;// need to free arr
			j++;
		}
	}
	return 1;
}

int helper(char **args ,char **env, t_vars var)
{
	char *path = retrieve_path(args[0],env);
	int pid = fork();
	if (pid == 0)
	{
		close_fd(var.fd, var.i, var.pipe_num);
		switch_fd(var.fd, var.i, var.pipe_num);
		if (execve(path, args, env) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}
	return pid;
}

void exec_pipe(char ***args, char **envp)
{
	t_vars var;

	init_var(&var, args);
	while (var.i < var.pipe_num) 
	{
		var.pid[var.i] = helper(args[var.i], envp, var);
    	var.i++;
	}
	var.i = 0;
	while(var.i < var.pipe_num)	
	{
		close(var.fd[var.i][0]);
		close(var.fd[var.i++][1]);
	}
	var.i = 0;
	int ec = waitpid(var.pid[var.pipe_num],NULL,0);
	while (var.i < var.pipe_num - 1)
	{
		wait(NULL);
		var.i++;
	}
	free_all(args);
}
