/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:45:00 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/06 20:35:08 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*substrdup(int start, int end, char *str)
{
	char	*new;
	int		i;

	new = (char *)ft_malloc(end - start + 1, 0);
	i = 0;
	while (start < end)
		new[i++] = str[start++];
	new[i] = '\0';
	return (new);
}

static int	ft_num_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*s;
	int		len;
	long	nb;

	nb = n;
	len = ft_num_len(nb);
	s = (char *)ft_malloc(len + 1, 0);
	s[len--] = '\0';
	if (n == 0)
		s[0] = '0';
	while (nb > 0)
	{
		s[len--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (s);
}
