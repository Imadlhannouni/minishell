#include "minishell.h"

void	unset(char ***env, char *arg)
{
	if (!arg)
		return;
	char **clone = malloc(var_num(*env) * sizeof(char*));
	int i = 0;
	int j = 0;

	while ((*env)[i])
	{
		if (strncmp((*env)[i], arg, ft_strlen(arg)) != 0)
			clone[j++] = (*env)[i];
		i++;
	}
	clone[j] = NULL;
	*env = clone;
}