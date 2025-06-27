#include "minishell.h"

// void	print_env(char **env)
// {
// 	int i = 0;
// 	while (env[i])
// 	{
// 		printf("%s\n", env[i]);
// 		i++;
// 	}
// }
// size_t var_num(char **arr)
// {
// 	int i = 0;

// 	if (!*arr)
// 		return 0;
// 	while (arr[i])
// 	{
// 		i++;
// 	}
// 	return i;
// }
void	print_sorted(char **env)
{
	int i = 0;
	char *temp;
	int len = var_num(env);
	while (i < len - 1)
	{
		if (env[i][0] > env[i + 1][0])
		{
			temp = env[i];
			env[i] = env[i + 1];
			env[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
	print_env(env);
}

void sort(char **arr, int pos)
{
	int i = 0, j = 0;
	char *temp;
	
	while (i < var_num(arr))
	{
		if (arr[i][0] == arr[i + 1][0])
		{
			while (j < pos)
			{
				if (arr[i][j] > arr[i + 1][j])
					break;
				j++;
			}
			temp = arr[i];
			arr[i] = arr[i + 1];
			arr[i + 1] = temp;
			i++;
		}
	}
	
}

// int	main(void)
// {
// char *values[] = {
// 	"ZEBRA=black",
// 	"APPLE=red",
// 	"BANANA=yellow",
// 	"CARROT=orange",
// 	"DATE=brown",
// 	"EGGPLANT=purple",
// 	"FIG=dark",
// 	"GRAPE=green",
// 	"IDHG",
// 	"MDOIJO",
// 	"FFF",
// 	"HONEY=golden",
// 	"ICE=transparent",
// 	"JUICE=orange",
// 	"KITE=colorful",
// 	"LEMON=yellow",
// 	"MANGO=orange",
// 	"NECTARINE=peach",
// 	"ORANGE=orange",
// 	"PINEAPPLE=brown",
// 	"QUINCE=yellow",
// 	"RASPBERRY=red",
// 	"STRAWBERRY=red",
// 	"TOMATO=red",
// 	"UGLI=green",
// 	"VANILLA=white",
// 	"WATERMELON=green",
// 	"XIGUA=red",
// 	"YAM=purple",
// 	"ZUCCHINI=green"
// };

// 	int		count = sizeof(values) / sizeof(values[0]);
// 	char	**env = malloc((count + 1) * sizeof(char *));
// 	int		i;

// 	if (!env)
// 		return (1);
// 	for (i = 0; i < count; i++)
// 	{
// 		env[i] = strdup(values[i]);
// 		if (!env[i])
// 			return (1);
// 	}
// 	env[i] = NULL;

// 	print_sorted(env);
// 	return 0;
// }
