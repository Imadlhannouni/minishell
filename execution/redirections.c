/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:17:54 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/07 12:04:31 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_red	*create_redirection(t_token *tok)
{
	t_red	*red;

	red = ft_malloc(sizeof(t_red), 0);
	red->next = NULL;
	red->is_ambigious = tok->ambigious;
	red->file = ft_strdup(tok->value);
	if (tok->inp_red || tok->heredoc)
		red->red_type = 0;
	else if (tok->out_red)
		red->red_type = 1;
	else if (tok->out_app)
		red->red_type = 2;
	return (red);
}

void	add_redirection(t_red **red, t_red *new_red)
{
	t_red	*tmp;

	if (!red || !new_red)
		return ;
	if (!*red)
	{
		*red = new_red;
		return ;
	}
	tmp = *red;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_red;
}

int	dup_output(char *file, int type)
{
	int	fd;

	if (type == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror("Minishell"), -1);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("Minishell"), -1);
	close(fd);
	return (1);
}

static int	help_redirect(t_red *red)
{
	int	fd;

	fd = -1;
	fd = open(red->file, O_RDONLY, 0644);
	if (fd < 0)
		return (perror("Minishell"), -1);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (perror("Minishell"), -1);
	close(fd);
	return (1);
}

int	handle_redirections(t_exe *var)
{
	int		fd1;
	t_red	*red;

	fd1 = -1;
	if (!var->redirections)
		return (1);
	red = var->redirections;
	while (red)
	{
		if (red->is_ambigious)
			return (putstr_fd("Minishell : ambiguous redirect\n", 2), -1);
		if (red->red_type == 0)
		{
			if (help_redirect(red) < 0)
				return (-1);
		}
		else if (red->red_type == 1 || red->red_type == 2)
		{
			if (dup_output(red->file, red->red_type) < 0)
				return (-1);
		}
		red = red->next;
	}
	return (1);
}
