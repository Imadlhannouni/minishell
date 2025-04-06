/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:42:05 by ilhannou          #+#    #+#             */
/*   Updated: 2025/04/05 17:33:18 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <string.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>

typedef struct s_line
{
	char	**full_line;
	char	*cmd;
	char	*option;
	char	*path;
}	t_line;

char	*ft_strchr(const char *s, int c);
char	**ft_split(const char *s, char c);
size_t	ft_strcpy(char *dst, const char *src);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *s);

#endif