NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline

SRC = main.c \
      built-ins/cd.c built-ins/echo.c built-ins/exit.c built-ins/export.c \
      built-ins/export_tools.c built-ins/print_env.c built-ins/print_env_sorted.c \
      built-ins/pwd.c built-ins/unset.c \
      execution/exec.c execution/exec1.c execution/ft_malloc.c \
      execution/lst_tools.c execution/pipe.c execution/redirections.c \
      execution/tools.c execution/tools2.c execution/tools3.c \
      parsing/env.c parsing/env2.c parsing/env3.c parsing/error_handling.c \
      parsing/error_handling2.c parsing/error_handling3.c \
      parsing/ft_strjoin_v2.c parsing/group_to_pipes.c \
      parsing/heredoc_parsing.c parsing/lst.c parsing/minishell_utils.c \
      parsing/minishell_utils2.c parsing/minishell_utils3.c parsing/parsing.c \
      parsing/read_heredoc.c parsing/read_heredoc2.c parsing/split.c \
      parsing/split_env.c parsing/split_env2.c parsing/tokenization.c \
      parsing/tokens_manipulation.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Compilation de minishell..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)
	@echo "Compilation terminée!"

%.o: %.c minishell.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Suppression des fichiers objets..."
	@rm -f $(OBJ)
	@echo "Nettoyage terminé!"

fclean: clean
	@echo "Suppression de l'exécutable..."
	@rm -f $(NAME)
	@echo "Nettoyage complet!"

re: fclean all

.PHONY: all clean fclean re
