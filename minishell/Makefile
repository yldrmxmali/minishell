NAME = minishell

CC = gcc
CFLAGS = -g -Wall -Wextra -Werror
LDFLAGS = -lreadline

RM = rm -rf

LIBFT = libft
LIBFTPATH = $(LIBFT)/libft.a

SRC = lexer.c \
      env.c \
      main.c \
      parser.c \
	  parser_utils.c \
	  free_parser.c \
      signal.c \
	  utils.c \
      free.c \
      expander.c \
	  expander_utils.c \
	  builtin_utils_exec.c \
	  builtin_utils_export.c \
	  builtin_utils_excont.c \
	  builtin_utils_unset.c \
	  builtin_utils.c \
	  builtin.c \
	  creat_files.c \
	  dup.c \
	  exec_utils.c \
	  exec.c \
	  execve.c \
	  execve_utils.c \
	  heredoc.c \
	  heredoc_utils.c


OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LIBFT)

	$(CC) $(CFLAGS) $(OBJ) $(LIBFTPATH) $(LDFLAGS) -o $(NAME)

clean:
	$(RM) $(OBJ)
	@make clean -C $(LIBFT)

fclean: clean
	$(RM) $(NAME)
	@make fclean -C $(LIBFT)


re: fclean all

.PHONY: all clean fclean re
