#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "minishell.h"


void	pwd(void)
{	
	char buffer[200] = {0};
	getcwd(buffer,200);
	printf("%s\n",buffer);
}