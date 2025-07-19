#include "../minishell.h"

t_exe *creat_node(t_token *tok)
{
	t_exe *node;

	node = malloc(sizeof(t_exe *));
	if (!node)
		return NULL;
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


