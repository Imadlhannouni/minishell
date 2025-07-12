/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 14:31:36 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/12 22:21:36 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*read_heredoc(char *delimiter)
{
	char	*line;
	char	*content;
	char	*tmp;

	content = ft_strdup("");
	
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		tmp = content;
		content = ft_strjoin(tmp, line);
		free(tmp);
		if (!content)
			return(free(content), NULL);
		tmp = content;
		content = ft_strjoin(tmp, "\n");
		free(tmp);
		if (!content)
			return(free(content), NULL);
		free(line);
	}
	return (content);
}

char	*create_heredoc_file(char *content)
{
	static int	heredoc_num = 0;
	char		*filename;
	int			fd;

	filename = ft_strjoin("/tmp/.heredoc_", ft_itoa(heredoc_num++));
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("Erreur lors de l'ouverture du fichier");
        return (NULL);
	}
	write(fd, content, ft_strlen(content));
	close(fd);
	return (filename);
}
