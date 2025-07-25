/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:17:54 by abbenmou          #+#    #+#             */
/*   Updated: 2025/07/25 12:41:32 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_red	*create_redirection(t_token *tok)
{
	t_red	*red;

	red = malloc(sizeof(t_red));
	if (!red)
		return NULL;
	red->next = NULL;
	red->file = ft_strdup(tok->value);
	if (!red->file)
		return NULL;
	if (tok->inp_red || tok->heredoc)
		red->red_type = 0;
	else if (tok->out_red)
		red->red_type = 1;
	else if (tok->out_app)
		red->red_type = 2;
	return red;
}

void	add_redirection(t_red **red, t_red *new_red)
{
	t_red *tmp;

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
int	fill_redirection(t_exe *var, t_token *tok)
{
	t_red *new_red;

	if (!(tok->heredoc != 0 || tok->inp_red != 0
		|| tok->out_app != 0 || tok->out_red != 0))
		return 1;
	new_red = create_redirection(tok);
	if (!new_red)
		return 0;
	add_redirection(&(var->redirections), new_red);
	return 1;
}

int	dup_output(char *file, int type)
{
	int	fd;
	
	if (type == 1)
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
	else
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0666);
	if (fd < 0)
		exit(1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return 1;
}

int	handle_redirections(t_exe *var)
{
	int fd1 = -1;
	t_red	*red;

	if (!var->redirections || !(var->redirections)->file)
		return 0;
	red = var->redirections;
	while (red)
	{
		if (red->red_type == 0)
		{
			// printf("%d\n",red->file);
			fd1 = open(red->file, O_RDWR , 0666);
			if (fd1 < 0)
				exit(1);
			dup2(fd1, STDIN_FILENO);
			close(fd1);
		}
		else if (red->red_type == 1 || red->red_type == 2)
			dup_output(red->file, red->red_type);
		red = red->next;
	}
	return 1;
}
