#include "../minishell.h"

static char	*allocate(size_t size)
{
	size_t	i;
	char	*full;

	i = 0;
	full = (char *)malloc(size * sizeof(char) + 1);
	if (!full)
		return (0);
	while (i < size)
	{
		full[i] = 0;
		i++;
	}
	return (full);
}

char	*ft_strjoin_v2(char *s1, char *s2, int flag)
{
	size_t	i;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = allocate(ft_strlen(s1) + ft_strlen(s2));
	if (!str)
		return (0);
	i = 0;
	while (i < ft_strlen(s1))
	{
		str[i] = s1[i];
		i++;
	}
	while (i < (ft_strlen(s1) + ft_strlen(s2)))
	{
		str[i] = s2[i - ft_strlen(s1)];
		i++;
	}
	str[i] = '\0';
	if (flag)
		free(s1);
	return (str);
}
