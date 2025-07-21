#include "../minishell.h"

int	fill_redirection(t_exe *var, t_token *tok)
{
	if (!(tok->heredoc != 0 || tok->inp_red != 0
		|| tok->out_app != 0 || tok->out_red != 0))
		return 1;
	if (tok->heredoc != 0 || tok->inp_red != 0)
	{
		var->in_red_type = 1;
		if (var->in_red_file)
			free(var->in_red_file);
		var->in_red_file = ft_strdup(tok->value);
		if (!var->in_red_file)
			return 0;
	}
	else if (tok->out_app != 0 || tok->out_red)
	{
		if (tok->out_app != 0)
			var->out_red_type = 1;
		else if (tok->out_red != 0)
			var->out_red_type = 2;
		if (var->out_red_file)
			free(var->out_red_file);
		var->out_red_file = ft_strdup(tok->value);
		if (!var->out_red_file)
			return 0;
	}
	return 1;
}

int	group_2d_arr(t_exe *var,t_token *tok)
{
	t_token	*temp;
	int i;

	i = 0;
	temp = tok;
	(var)->arr = malloc((count_args(tok) + 1) * sizeof(char*));
	if (!(var)->arr)
		return 0;
	while (temp)
	{
		if ((temp->inp_red == 0) && (temp->heredoc == 0)
			&& (temp->out_red == 0) && (temp->out_app == 0))
			(var)->arr[i++] = ft_strdup(temp->value);
		if (!(var)->arr[i - 1])
			return (free_arr((var)->arr, i - 2), 0);
		if(!fill_redirection(var, temp))
			return (free_2d_arr((var)->arr), 0);
		temp = temp->next;
	}
	(var)->arr[i] = NULL;
	return 1;
}

void exec_command(t_exe *var, char **env)
{
	int pid;
	int fd[2] = {0 , 0};
	char *path = retrieve_path(var->arr[0],env);
	if (!path)
	 	return;
	pid = fork();
	if (pid == 0)
	{
		handle_redirections(var, fd);
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
		// printf("red_in_type == %d || red_in_file == %s\n",var->in_red_type,var->in_red_file);
		// printf("red_out_type == %d || red_out_file == %s\n",var->out_red_type,var->out_red_file);
		if (!is_builtin(var->arr[0]))
			exec_command(var, *env);
		else if(is_builtin(var->arr[0]))
			exec_builtin(var, env, &no_val, fd);
		if (fd[0] >= 0 || fd[1] >= 0)
		{
			if (var->out_red_type)
				dup2(fd[1] ,STDOUT_FILENO);
			if (var->in_red_type)
				dup2(fd[0] ,STDIN_FILENO);
		}
	}
	free_t_exe(&var);
}
