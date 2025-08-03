/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:35:32 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/03 13:46:52 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_malloc(t_malloc **help)
{
	t_malloc *tmp;
	t_malloc *tmp1;
	
	if (!*help)
		return ;
	tmp1 = *help;
	while (tmp1)
	{
		tmp = tmp1->next;
		free(tmp1->adress);
		free(tmp1);
		tmp1 = tmp;
	}
}

static t_malloc *creat_adress(void *adress, t_malloc **head)
{
	t_malloc *node;

	if (!adress)
		return NULL;
	node = malloc(sizeof(t_malloc));
	if (!node)
	{
		free_malloc(head);
		exit(1);
	}
	node->adress = adress;
	node->next = NULL;
	return node;
}
 
static void add_to_malloc(t_malloc **lst, t_malloc *node)
{
	t_malloc *temp;

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

void	*ft_malloc(size_t len, int flag)
{
	static t_malloc	*head = NULL;
	void *adress;

	if (flag)
	{
		free_malloc(&head);
		return NULL;
	}
	adress = malloc(len);
	if (!adress)
	{
		free_malloc(&head);
		exit(1);
	}
	else
		add_to_malloc(&head, creat_adress(adress, &head));
	return adress;
}

void	exit_free(int exit_code)
{
	ft_malloc(0, 1);
	exit(exit_code);
}