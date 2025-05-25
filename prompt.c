/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:58:22 by ilhannou          #+#    #+#             */
/*   Updated: 2025/05/25 22:15:59 by ilhannou         ###   ########.fr       */
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

void	add_token(t_token **head, char *value, t_token_type type, int is_full)
{
	t_token *new;
	t_token *tmp;

	new = malloc(sizeof(t_token));
	new->type = type;
	new->value = value;
	new->inp_app = 0;
	new->inp_red = 0;
	new->out_app = 0;
	new->out_red = 0;
	new->is_fullstring = is_full;
	new->next = NULL;
	if (!*head)
		*head = new;
	else {
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	add_redirection(t_token **head, int	flag)
{
	t_token *tmp;

	tmp = *head;
	if (!tmp)
		return ;
	while (tmp->next)
		tmp = tmp->next;
	if (flag == 0)
		tmp->out_red = 1;
	else if (flag == 1)
		tmp->inp_red = 1;
	else if (flag == 2)
		tmp->out_app = 1;
	else
		tmp->inp_app = 1;
}

void	add_env(t_env **head, char *value, char *key)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->value = ft_strdup(value);
	new->key = ft_strdup(key);
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_token	*concat_fullstring(t_token *start, t_token **next)
{
	char	*joined;
	char	*tmp;
	t_token	*curr;
	t_token	*to_free;

	curr = start;
	joined = ft_strdup(curr->value);
	while (curr->is_fullstring == 1)
	{
		curr = curr->next;
		tmp = joined;
		joined = ft_strjoin(tmp, curr->value);
		free(tmp);
	}
	if (curr->out_red == 1)
		start->out_red = 1;
	else if (curr->inp_red == 1)
		start->inp_red = 1;
	else if (curr->out_app == 1)
		start->out_app = 1;
	else if (curr->inp_app == 1)
		start->inp_app = 1;
	curr = start->next;
	while (curr && curr->is_fullstring == 1)
	{
		to_free = curr;
		curr = curr->next;
		free(to_free->value);
		free(to_free);
	}
	if (curr && curr->is_fullstring == 0)
	{
		to_free = curr;
		curr = curr->next;
		free(to_free->value);
		free(to_free);
	}
	free(start->value);
	start->value = joined;
	start->next = curr;
	if (next)
		*next = curr;
	return (start);
}

void	add_pipe(t_pipe **head, t_token *fullcmd)
{
	t_pipe	*new;
	t_pipe	*tmp;
	t_token	*curr;
	t_token	*prev;
	t_token	*compact;

	new = malloc(sizeof(t_pipe));
	if (!new)
		return ;
	curr = fullcmd;
	prev = NULL;
	while (curr)
	{
		if (curr->is_fullstring == 1)
		{
			compact = concat_fullstring(curr, &curr);
			compact->type = 9;
			if (prev)
				prev->next = compact;
			else
				fullcmd = compact;
			prev = compact;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
	new->full_cmd = fullcmd;
	new->nextpipe = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->nextpipe)
			tmp = tmp->nextpipe;
		tmp->nextpipe = new;
	}
}

t_pipe	*group_tokens_into_pipes(t_token *tokens)
{
	t_pipe	*pipes;
	t_token	*start;
	t_token	*curr;
	t_token	*prev;

	pipes = NULL;
	start = tokens;
	curr = tokens;
	prev = NULL;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE)
		{
			curr->value = NULL;
			if (prev)
				prev->next = NULL;
			add_pipe(&pipes, start);
			start = curr->next;
		}
		prev = curr;
		curr = curr->next;
	}
	if (start)
		add_pipe(&pipes, start);
	return (pipes);
}

void print_pipes(t_pipe *pipes)
{
	t_pipe *curr_pipe = pipes;
	t_token *curr_token;
	int i;

	while (curr_pipe)
	{
		printf("=== New Pipe Command ===\n");
		curr_token = curr_pipe->full_cmd;
		while (curr_token)
		{
			printf("%s : %d : %d | inp_red : %d, out_red : %d,  inp_app : %d, out_app : %d\n", curr_token->value, curr_token->type, curr_token->is_fullstring, curr_token->inp_red, curr_token->out_red, curr_token->inp_app, curr_token->out_app);
			curr_token = curr_token->next;
		}
		curr_pipe = curr_pipe->nextpipe;
	}
}


char	*substrdup(int start, int end, char	*str)
{
	char	*new;
	int		i;
	
	new = malloc(end - start + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (start < end)
		new[i++] = str[start++];
	new[i] = '\0';
	return (new);
}

t_token	*smart_split(char *line, t_pipe *pipe, t_env **env)
{
	t_token			*tokens;
	t_token_type	type;
	t_token			*last;
	int				i;
	int				new_command;
	int				new_env;

	tokens = NULL;
	i = 0;
	new_command = 1;
	new_env = 1;
	if (character_count(line) == 0)
		return (tokens); // Error here
	while (line[i] != '\0')
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (!line[i])
			break ;
		if (new_command && line[i] != '>' && line[i] != '<' && line[i] != '|')
		{
			i = is_cmds_var(&tokens, type, i, line);
			new_command = 0;
		}
		else if (line[i] == '\'')
			i = is_simple_quote(tokens, type, i, line);
		else if (line[i] == '"')
			i = is_double_quote(tokens, type, i, line);
		else if (line[i] == '<' || line[i] == '>')
			i = is_directions(tokens, type, i, line);
		else if (line[i] == '|')
		{
			i = is_pipe(tokens, type, i, line);
			add_pipe(&pipe, tokens);
			new_command = 1;
			new_env = 1;
		}
		else if (line[i] == '-' && line[i + 1] == 'n' && (line[i + 2] == ' ' || line[i + 2] == '\t' || line[i + 2] == '\0'))
			i = is_option(tokens, type, i, line);
		else
		{
			if (line[i] == '\0')
				break ;
			i = is_word(tokens, type, i, line);
		}
	}
	return (tokens);
}

void	replace_env_variables(t_token *tokens, t_env *env)
{
	t_token	*curr;
	t_env	*curr_env;
	char	*key;
	char	*chunk;
	char	*result;
	char	*tmp;
	int		i;
	int		start;

	while (tokens)
	{
		if (tokens->value && strchr(tokens->value, '$'))
		{
			i = 0;
			result = ft_strdup("");
			while (tokens->value[i])
			{
				if (tokens->value[i] == '$')
				{
					i++;
					start = i;
					while (ft_isalnum(tokens->value[i]) || tokens->value[i] == '_')
						i++;
					key = substrdup(start, i, tokens->value);
					curr_env = env;
					while (curr_env)
					{
						if (ft_strcmp(curr_env->key, key) == 0)
							break;
						curr_env = curr_env->next;
					}
					if (curr_env)
					{
						tmp = result;
						result = ft_strjoin(tmp, curr_env->value);
						free(tmp);
					}
					free(key);
				}
				else
				{
					start = i;
					while (tokens->value[i] && tokens->value[i] != '$')
						i++;
					chunk = substrdup(start, i, tokens->value);
					tmp = result;
					result = ft_strjoin(tmp, chunk);
					free(tmp);
					free(chunk);
				}
			}
			free(tokens->value);
			tokens->value = result;
		}
		tokens = tokens->next;
	}
}

void	parsing(char *line, t_env **env)
{
	t_token	*tokens;
	t_pipe	*pipes;

	pipes = NULL;
	tokens = smart_split(line, pipes, env);
	if (!tokens)
		free_tokens(tokens);
	replace_env_variables(tokens, *env);
	pipes = group_tokens_into_pipes(tokens);
	is_path(pipes);
	print_pipes(pipes);
	return ;
}

t_env	*init_env(char **envp, t_env **env)
{
	int		i;
	char	**split;

	i = 0;
	while (envp[i])
	{
		split = ft_split(envp[i], '=');
		if (split && split[0])
		{
			if (split[1])
				add_env(env, split[1], split[0]);
			else
				add_env(env, "", split[0]);
		}
		ft_free_split(split, 2);
		i++;
	}
	return (*env);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env;

	(void)argc;
	(void)argv;
	env = NULL;
	init_env(envp, &env);
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
	line = NULL;
	while (1)
	{
		line = readline_func(line);
		if (!line)
			break;
		parsing(line, &env);
		free(line);
	}
	return (0);
}
