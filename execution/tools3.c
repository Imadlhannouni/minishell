/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 13:41:51 by abbenmou          #+#    #+#             */
/*   Updated: 2025/08/07 16:09:06 by abbenmou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	dup_std(int fd[2], t_help *help)
{
	help->std_fd = fd;
	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);
	if (fd[0] < 0 || fd[1] < 0)
		return (perror("Minishell"), -1);
	return (1);
}

void	close_previous(int (*fd)[2], int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

int	is_path1(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static void	*ft_memset(void *dest, int a, size_t n)
{
	size_t			i;
	unsigned char	*buf;

	i = 0;
	buf = (unsigned char *) dest;
	while (i < n)
	{
		buf[i] = (unsigned char)a;
		i++;
	}
	return (dest);
}

int	check_directory(char *path)
{
	struct stat	info;

	ft_memset(&info, 0, sizeof(info));
	stat(path, &info);
	if (S_ISDIR(info.st_mode))
		return (1);
	return (0);
}
