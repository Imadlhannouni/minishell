NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIBS = -lreadline

SRC = main.c \
      $(wildcard built-ins/*.c) \
      $(wildcard execution/*.c) \
      $(wildcard parsing/*.c)

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