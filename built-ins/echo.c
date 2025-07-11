#include "../minishell.h"

void echo(char **arg)
{
	int i = 3;
	char *temp;
	if (!arg[2])
		printf("%s\n",arg[1]);
	else
	{
		char *str = join_strings(arg[1], " ",arg[2]);

		while (arg[i])
		{
			temp = str;
			str = join_strings(str, " ",arg[i]);
			free(temp);
			i++;
		}
		printf("%s\n",str);
	}
}