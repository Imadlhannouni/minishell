/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 20:32:56 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/04 14:58:22 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_existence(char **env, char *name)
{
	int i ,len;

	i = 0;
	if (!env)
		return 0;
	len = ft_strlen((const char*)name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0)
				return 1;
		i++;
	}
	return 0;
}

char *join_strings(char *s1, char *s2, char *s3)
{
	char *temp;
	char *temp1;

	temp = ft_strjoin(s1, s2);
	temp1  = ft_strjoin(temp, s3);
	return (temp1);
}

static int	check_char(char c)
{
	if (!((c >= 48 && c <= 57) ||
		(c >= 65 && c <= 90) ||
		(c >= 97 && c <= 122) ||
		c == 95 || c == '='))
		return 0;
	return 1; 
}

int check_var(char *str)
{
	int i = 0;
	if (!((str[i] >= 65 && str[i] <= 90) ||
		(str[i] >= 97 && str[i] <= 122) ||
		str[i] == 95 || str[i] == '='))
		return 0;
	while (str[i])
	{
		if (!check_char(str[i]))
			return 0;
		i++;
	}
	return 1;
}

char **spec_split(char *str)
{
	char **arr = NULL;
	int i = 0;

	arr = ft_malloc(3 * sizeof(char*), 0);
	while (str[i] && str[i] != '=')
		i++;
	arr[0] = fill_word(0, i, str);
	arr[1] = fill_word(++i, ft_strlen(str), str);
	arr[2] = NULL;
	return arr;
}
