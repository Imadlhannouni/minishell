/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:08:05 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/05 17:08:57 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redir_error_angle_bracket(char *s, int *i_ptr, int i)
{
	i += 2;
	i = skip_spaces(s, i);
	if (!s[i] || s[i] == '|' || s[i] == '<' || s[i] == '>')
		return (ft_putstr_fd("minishell: syntax error\n", 2), 1);
	*i_ptr = i;
	return (0);
}
