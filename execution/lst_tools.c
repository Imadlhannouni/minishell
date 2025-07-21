#include "../minishell.h"

t_exe *creat_node(t_token *tok)
{
	t_exe *node;

	if (!tok)
		return NULL;
	node = malloc(sizeof(t_exe));
	if (!node)
		return NULL;
	node->arr = NULL;
	node->in_red_file = NULL;
	node->out_red_file = NULL;
	node->in_red_type = 0;
	node->out_red_type = 0;
	if (!group_2d_arr(node,tok))
		return (free(node), NULL);
	node->next = NULL;
	return node;
}

void add_node(t_exe **lst, t_exe *node)
{
	t_exe *temp;

	if (!lst || !node)
		return;
	if (!*lst)
	{
		*lst = node;
		return;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = node;
}

void	free_t_exe(t_exe **var)
{
	t_exe *tmp;

	while (*var)
	{
		tmp = *var;
		*var = (*var)->next;
		free_2d_arr(tmp->arr);
		if (tmp->in_red_file)
			free(tmp->in_red_file);
		if (tmp->out_red_file)
			free(tmp->out_red_file);
		free(tmp);
	}
}
