/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:42:05 by ilhannou          #+#    #+#             */
/*   Updated: 2025/08/06 22:45:50 by ilhannou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

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
	int				ambigious;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_heredoc
{
	char			*delimiter;
	t_token_type	type;
	char			*filename;
	int				write_fd;
}					t_heredoc;

typedef struct s_expand_ctx
{
	char			**clone_envi;
	char			*exit_code;
}					t_expand_ctx;

typedef struct s_help
{
	void			(*prev_handler_int)(int);
	void			(*prev_handler_quit)(int);
	char			**exit_code;
	int				*std_fd;
	int				child;
}					t_help;

typedef struct s_pipe
{
	t_token			*full_cmd;
	int				ambigious;
	struct s_pipe	*nextpipe;
}					t_pipe;

typedef struct s_malloc
{
	void			*adress;
	struct s_malloc	*next;
}					t_malloc;

typedef struct s_vars
{
	size_t			i;
	size_t			pipe_num;
	int (*fd)[2];
	__pid_t			*pid;
}					t_vars;

typedef struct s_red
{
	int				red_type;
	char			*file;
	struct s_red	*next;
}					t_red;

typedef struct s_exe
{
	char			**arr;
	t_red			*redirections;
	int				is_ambiguis;
	struct s_exe	*next;
}					t_exe;

long				ft_atoi(const char *str);
char				*ft_strchr(const char *s, int c);
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
int					add_token(t_token **head, char *value, t_token_type type,
						int is_full);
char				*substrdup(int start, int end, char *str);
int					is_simple_quote(t_token **tokens, int i, char *line,
						int *flag);
int					is_double_quote(t_token **tokens, int i, char *line,
						int *flag);
int					is_directions(int i, char *line, int *flag);
int					is_pipe(t_token *tokens, int i, char *line);
int					is_word(t_token **tokens, int i, char *line, int *flag);
int					replace_env_variables(t_pipe **pipes, char **clone_envi,
						char **exit_code);
char				**ft_split(const char *s, char c);
char				**ft_split_env(const char *s, char c);
t_pipe				*group_tokens_into_pipes(t_token *tokens);
void				print_pipes(t_pipe *pipes);
t_token				*concat_fullstring(t_token *start, t_token **next);
int					main_parsing(char *line, char **clone_envi, t_pipe **pipes,
						char **exit_code);
void				print_cmd_not_found(char *cmd);
char				*read_heredoc(char *delimite, char **clone_envi,
						t_token_type type, char **exit_code);
char				*ft_itoa(int n);
int					append_env_or_chunk(char *str, int i, t_expand_ctx *ctx,
						char **result);
void				cleanup_heredoc_files(t_pipe *pipes);
int					handle_errors(char *line, char **exit_code);
char				*ft_itoa(int n);
void				apply_flag_to_token(t_token *last, int *flag,
						int set_expand);
void				compact_fullstrings(t_token **fullcmd);
char				**ft_split2(const char *s, char c);
int					global_var2(int new_value);
void				expand_and_write_line(struct s_heredoc *heredoc, char *line,
						char **exit_code, char **clone_envi);
void				cleanup_heredoc_files(t_pipe *pipes);
int					handle_pipe_error(char *s, int i, int has_cmd);
int					check_basics(char *line);
int					skip_word(char *s, int i);
int					skip_spaces(char *s, int i);
char				*free_line(char *new_value, int i);
int					process_heredoc_token(t_token *current_token,
						char **clone_envi, char **exit_code);
int					handle_heredoc_tokens(t_token *token, char **clone_envi,
						char **exit_code);
int					handle_heredoc_token(t_token *current_token,
						char **clone_envi, char **exit_code);
int					handle_env_var(char *str, int i, t_expand_ctx *ctx,
						char **result);
int					handle_chunk(char *str, int i, char **result);
void				handle_token_split(t_token *tokens, char *expanded);
char				*get_env_value(const char *key, char **clone_envi);
int					randomid(void);
int					handle_redir_error_angle_bracket(char *s, int *i_ptr, int i);

int					ft_isdigit(int a);
int					is_space(char c);
void				sighandler(int signum);
void				*ft_malloc(size_t len, int flag);
int					extract_status(int status);
int					check_equ(char *str);

int					execute(t_pipe *pipes, char ***env, t_help *help);
int					exec_pipe(t_exe *var, char ***envp, size_t pipe_num,
						t_help *help);
int					exec_command(t_exe *var, char **env, t_help *help);
int					exec_builtin(t_exe *var, char ***env, t_help *help);
int					check_directory(char *path);
void				check_path(char *path, int *e_code);
int					is_path1(char *cmd);
int					dup_std(int fd[2], t_help *help);

size_t				var_num(char **arr);
char				**clone_env(char **env);
void				putstr_fd(char *str, int fd);
char				*ft_strjoin_v2(char *s1, char *s2, int flag);
int					retrieve_path(char *cmd, char **env, char **path);
int					count_args(t_token *tok);
int					count_pipes(t_pipe *pipes);
void				close_fd(int (*fd)[2], size_t i, size_t total);
void				switch_fd(int (*fd)[2], size_t i, size_t total);
void				close_previous(int (*fd)[2], int j);
int					helper(t_exe *tmp, char ***env, t_vars var, t_help *help);
int					init_var(t_vars *var, size_t pipe_num);

int					print_sorted(char **env);
int					cd(char **arr, char ***env);
int					pwd(char **env);
int					print_env(char **env, char **args);
int					unset(char ***env, char **var);
int					echo(char **arg);
int					export(char ***env, char **args);
char				**spec_split(char *str);
int					check_var(char *str);
char				*join_strings(char *s1, char *s2, char *s3);
int					check_existence(char **env, char *name);
char				*fill_word(int start, int end, char *str);
void				group_pipes(t_pipe *pipes, t_exe **var);
void				check_ambigious(t_exe *var);

int					check_equ(char *str);
int					is_builtin(char *cmd);
int					group_2d_arr(t_exe *var, t_token *tok);
int					fill_redirection(t_exe *var, t_token *tok);
t_exe				*creat_node(t_token *tok);
void				add_node(t_exe **lst, t_exe *node);
int					handle_redirections(t_exe *var);
int					reset_redirections(int fd[2]);
int					is_path1(char *cmd);
void				add_redirection(t_red **red, t_red *new_red);
int					fill_redirection(t_exe *var, t_token *tok);
int					exit_shell(t_exe *var, t_help *help);
void				exit_free(int exit_code);
int					helper_built_in(t_exe *var, char ***env, t_help *help);

#endif