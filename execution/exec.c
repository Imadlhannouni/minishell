#include "../minishell.h"

int	fill_redirection(t_exe *var, t_token *tok)
{
	if (!(tok->heredoc != 0 || tok->inp_red != 0
		|| tok->out_app != 0 || tok->out_red != 0))
	{
		(var)->red_file = NULL;
		(var)->red_type = 0;
		return 1;
	}
	if ((var)->red_file)
		free((var)->red_file);
	if (tok->heredoc != 0)
		(var)->red_type = 1;
	else if (tok->inp_red != 0)
		(var)->red_type = 2;
	else if (tok->out_app != 0)
		(var)->red_type = 3;
	else if (tok->out_red != 0)
		(var)->red_type = 4;
	(var)->red_file = ft_strdup(tok->value);
	if (!(var)->red_file)
		return 0;
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
		if(!fill_redirection(var, tok))
			return (free_2d_arr((var)->arr), 0);
		temp = temp->next;
	}
	(var)->arr[i] = NULL;
	return 1;
}

void exec_command(char **arg, char **env)
{
	int pid;
	char *path = retrieve_path(arg[0],env);
	if (!path)
	 	return;
	pid = fork();
	if (pid == 0)
	{
		execve(path, arg, env);
	}
	else
	{
		waitpid(pid, NULL, 0);
	}
	free(path);
}
void print_2d(char **arr)
{
	int i = 0;
	while (arr[i])
	{
		printf("%s\n",arr[i++]);
	}
	
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
	while (*var)
	{
		print_2d((*var)->arr);
		printf("-------\n");
		*var = (*var)->next;
	}
	
}

void	execute(t_pipe *pipes, char ***env)
{
	t_exe	*var = NULL;
	size_t count;
	// static char **no_val = NULL;
	(void)env;
	count = count_pipes(pipes);

	if (count > 1)
	{
		group_pipes(pipes, &var);
		// while (var)
		// {
		// 	int i = 0;
		// 	while (var->arr[i])
		// 	{
		// 		printf("||   %s   ||",var->arr[i++]);
		// 	}
		// 	write(1, "\n", 1);
		// 	var = var->next;
		// }
		
		//exec_pipe(var, env, &no_val, count);
	}
	// else if (count == 1)
	// {
	// 	char **arg = group_2d_arr(pipes->full_cmd);
	// 	if (!is_builtin(arg[0]))
	// 		exec_command(arg, *env);
	// 	else
	// 		exec_builtin(arg,env, &no_val);
	// 	free_2d_arr(arg);
	// }
}
