/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abbenmou <abbenmou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:42:05 by ilhannou          #+#    #+#             */
/*   Updated: 2025/07/30 19:19:26 by abbenmou         ###   ########.fr       */
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
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>
# include <dirent.h>

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_SIMPLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_REDIRECTION,
	TOKEN_PIPE,
	TOKEN_PATH,
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
	int				expand;
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

typedef struct s_red
{
	int		red_type;
	char	*file;
	struct s_red *next;
}	t_red;

typedef struct s_exe
{
	char **arr;
	t_red	*redirections;
	struct s_exe *next;
}	t_exe;

typedef struct s_free
{
	t_pipe	*pipes;
	t_exe	*exe;
	char **env;
	char **no_val;
	int	*pid;
	int (*fd)[2];
	int *fds;
	char *path;
	char **exit_code;
	void (*prev_handler_int)(int);
	void (*prev_handler_quit)(int);
}	t_free;

char				*ft_strchr(const char *s, int c);
size_t				ft_strcpy(char *dst, const char *src);
char				*ft_strdup(const char *s1);
size_t				ft_strlen(const char *s);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strjoin(char const *s1, char const *s2);
void				ft_putstr_fd(char *s, int fd);
void				ft_putchar_fd(char c, int fd);
int					character_count(char *s);
int					ft_isalpha(char *s);
int					ft_isalnum(int c);
t_token				*ft_lstnew(char *value);
t_token				*ft_lstlast(t_token *lst);
void				ft_lstadd_back(t_token **lst, t_token *new);
void				add_token(t_token **head, char *value, t_token_type type,
						int is_full);
char				*substrdup(int start, int end, char *str);
int					is_simple_quote(t_token **tokens, int i,
						char *line, int *flag);
int					is_double_quote(t_token **tokens, int i,
						char *line, int *flag);
int					is_directions(int i, char *line,
						int *flag);
int					is_pipe(t_token *tokens, int i,
						char *line);
int					is_word(t_token **tokens, int i,
						char *line, int *flag);
void				free_tokens(t_token *tokens);
void				free_pipes(t_pipe **pipes);
int					replace_env_variables(t_pipe **pipes, char **clone_envi, char *exit_code);
char				**ft_split(const char *s, char c);
char				**ft_split_env(const char *s, char c);
t_pipe				*group_tokens_into_pipes(t_token *tokens);
void				print_pipes(t_pipe *pipes);
t_token				*concat_fullstring(t_token *start, t_token **next);
int					main_parsing(char *line, char **clone_envi, t_pipe **pipes, char *exit_code);
void				print_cmd_not_found(char *cmd);
char				*read_heredoc(char *delimite, char **clone_envi, t_token_type type, char **exit_code);
char				*ft_itoa(int n);
char				*create_heredoc_file(char *content);
int					append_env_or_chunk(char *str, int i, char **clone_envi,
						char **result, char *exit_code);
void				cleanup_heredoc_files(t_pipe *pipes);
int					handle_errors(char *line);
char				*ft_itoa(int n);
void				apply_flag_to_token(t_token *last, int *flag, int set_expand);
void				compact_fullstrings(t_token **fullcmd);

size_t	var_num(char **arr);
void	free_arr(char **arr, int j);
char	**clone_env(char **env);
int		execute(t_pipe *pipes, char ***env, t_free *collect);
void 	putstr_fd(char *str, int fd);
char	*ft_strjoin_v2(char *s1, char *s2, int flag);
char	*retrieve_path(char *cmd, char **env, t_free *collect);
void	free_2d_arr(char **arr);
int		count_args(t_token *tok);
int		count_pipes(t_pipe *pipes);
int		exec_pipe(t_exe *var, char ***envp, size_t pipe_num, t_free *collect);
void	close_fd(int (*fd)[2], size_t i, size_t total);
void	switch_fd(int (*fd)[2], size_t i, size_t total);
void	close_previous(int (*fd)[2], int j);
int		helper(t_exe *tmp ,char ***env, t_vars var, t_free *collect);
int		init_var(t_vars *var, size_t pipe_num, t_free *collect);
int 	exec_command(t_exe *var, char **env, t_free *collect);


int		print_sorted(char **env, char **arr);
int		cd(char **arr, char ***env, t_free *collect);
void	exit_free(t_free *collect, int exit_code);
int		pwd(void);
int		print_env(char **env, char **args);
int		unset(char ***env, char **var, char ***no_val);
int		echo(char **arg);
int		export(char ***env, char **args, char ***no_val);
char	**spec_split(char *str);
int		check_var(char *str);
char	*join_strings(char *s1, char *s2, char *s3);
int		check_existence(char **env, char *name);
char	*fill_word(int start, int end, char *str);
void	group_pipes(t_pipe *pipes, t_exe **var, t_free *collect, char **env);
int		exec_builtin(t_exe *var, char ***env, t_free *collect);

int 	is_builtin(char *cmd);
int		group_2d_arr(t_exe *var, t_token *tok);
int		fill_redirection(t_exe *var, t_token *tok);
t_exe	*creat_node(t_token *tok);
void	add_node(t_exe **lst, t_exe *node);
void	free_t_exe(t_exe **var);
int		handle_redirections(t_exe *var);
void	reset_redirections(int fd[2]);
int		is_path1(char *cmd);
void	add_redirection(t_red **red, t_red *new_red);
int		fill_redirection(t_exe *var, t_token *tok);
void	exit_shell(t_exe *var, t_free *collect);

#endif