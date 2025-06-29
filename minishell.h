/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilhannou <ilhannou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:42:05 by ilhannou          #+#    #+#             */
/*   Updated: 2025/06/29 14:35:44 by ilhannou         ###   ########.fr       */
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

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

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
int					is_directions(t_token *tokens, int i, char *line,
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
void				free_env(t_env *env);
void				add_env(t_env **head, char *value, char *key);
void				replace_env_variables(t_token *tokens, t_env *env);
t_env				*init_env(char **envp, t_env **env);
char				**ft_split(const char *s, char c);
void				ft_free_split(char **split, int j);
void				add_redirection(t_token **head, int flag);
t_pipe				*group_tokens_into_pipes(t_token *tokens);
void				print_pipes(t_pipe *pipes);
t_token				*concat_fullstring(t_token *start, t_token **next);
void				main_parsing(char *line, t_env **env, t_pipe **pipes);
void				print_cmd_not_found(char *cmd);
void				print_syntax_error(char *token);

#endif