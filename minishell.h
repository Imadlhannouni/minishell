/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:42:05 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/19 16:35:56 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>

typedef enum s_token_type
{
	TOKEN_CMD,
	TOKEN_WORD,
	TOKEN_VAR,
	TOKEN_SIMPLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_REDIRECTION,
	TOKEN_PIPE,
	TOKEN_PATH,
	TOKEN_OPTION,
	TOKEN_LINKEDSTRING
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	int				is_fullstring;
	int				inp_red;
	int				out_red;
	int				heredoc;
	int				out_app;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_pipe
{
	t_token			*full_cmd;
	struct s_pipe	*nextpipe;
}					t_pipe;

typedef struct s_vars
{
	size_t i;
	size_t pipe_num;
	int (*fd)[2];
	__pid_t *pid;
}	t_vars;

typedef struct s_exe
{
	char **arr;
	int in_red_type;
	int out_red_type;
	char *out_red_file;
	char *in_red_file;
	struct s_exe *next;
}	t_exe;


char				*ft_strchr(const char *s, int c);
size_t				ft_strcpy(char *dst, const char *src);
char				*ft_strdup(const char *s1);
size_t				ft_strlen(const char *s);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strjoin(char const *s1, char const *s2);
void				ft_putstr_fd(char *s, int fd);
int					character_count(char *s);
int					ft_isalpha(char *s);
int					ft_isalnum(int c);
t_token				*ft_lstnew(char *value);
t_token				*ft_lstlast(t_token *lst);
void				ft_lstadd_back(t_token **lst, t_token *new);
void				add_token(t_token **head, char *value, t_token_type type,
						int is_full);
char				*substrdup(int start, int end, char *str);
int					is_cmds_var(t_token **tokens, int i,
						char *line, int *flag);
int					is_simple_quote(t_token *tokens, int i,
						char *line, int *flag);
int					is_double_quote(t_token *tokens, int i,
						char *line, int *flag);
int					is_directions(int i, char *line,
						int *flag);
int					is_pipe(t_token *tokens, int i,
						char *line);
int					is_word(t_token *tokens, int i,
						char *line, int *flag);
int					is_option(t_token *tokens, int i,
						char *line);
void				is_path(t_pipe *pipe);
void				free_tokens(t_token *tokens);
void				free_pipes(t_pipe **pipes);
void				replace_env_variables(t_token *tokens, char **clone_envi);
char				**ft_split(const char *s, char c);
char				**ft_split_env(const char *s, char c);
t_pipe				*group_tokens_into_pipes(t_token *tokens);
void				print_pipes(t_pipe *pipes);
t_token				*concat_fullstring(t_token *start, t_token **next);
int					main_parsing(char *line, char **clone_envi, t_pipe **pipes);
void				print_cmd_not_found(char *cmd);
void				print_syntax_error(char *token);
char				*read_heredoc(char *delimite, char **clone_envi, t_token_type type);
char				*ft_itoa(int n);
char				*create_heredoc_file(char *content);
int					append_env_or_chunk(char *str, int i, char **clone_envi,
						char **result);
void				cleanup_heredoc_files(t_pipe *pipes);

size_t 	var_num(char **arr);
void	free_arr(char **arr, int j);
char 	**clone_env(char **env);
void	execute(t_pipe *pipes, char ***env);

char	*ft_strjoin_v2(char *s1, char *s2, int flag);
char	*retrieve_path(char *cmd, char **env);
void	free_2d_arr(char **arr);
int		count_args(t_token *tok);
int		count_pipes(t_pipe *pipes);
void	exec_pipe(t_exe *var, char ***envp, char ***no_val, size_t pipe_num);
void	close_fd(int (*fd)[2], size_t i, size_t total);
void	switch_fd(int (*fd)[2], size_t i, size_t total);
void 	close_all(int (*fd)[2], int j);

void	print_sorted(char **env, char **arr);
void	cd(char *path, char ***env);
char 	*retreive_var(char **env, char *var);
void	pwd(void);
void	print_env(char **env);
void	export(char ***env, char **args, char ***no_val);
void	unset(char ***env, char *var);
char 	**spec_split(char *str);
void 	echo(char **arg);
char 	*join_strings(char *s1, char *s2, char *s3);
int 	is_builtin(char *cmd);
void 	exec_builtin(char **arg, char ***env, char ***no_val);
int		group_2d_arr(t_exe *var,t_token *tok);
int		fill_redirection(t_exe *var, t_token *tok);
t_exe 	*creat_node(t_token *tok);
void 	add_node(t_exe **lst, t_exe *node);
void	free_t_exe(t_exe **var);
int handle_redirections(t_exe *var);

#endif