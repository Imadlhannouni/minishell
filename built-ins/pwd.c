#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "../minishell.h"


void	pwd(void)
{	
	char *buffer;
	
	buffer = getcwd(NULL,0);
	printf("%s\n",buffer);
}