CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = free_env.c minishell.c utils_env.c quick_sort.c\
	  utils_export.c mini_shell_split.c expander.c expander_utils.c\
	  builtins/cd.c builtins/pwd.c builtins/echo.c builtins/env.c \
	  builtins/exit.c builtins/export.c builtins/unset.c builtins/clear.c\

OBJCS = ${SRC:.c=.o}

NAME = minishell
LIBFT_DIR = libft
LIBFT = ${LIBFT_DIR}/libft.a
LDFLAGS = -lreadline -ltinfo

all: ${NAME}

${NAME}: ${OBJCS} ${LIBFT}
	${CC} ${CFLAGS} ${OBJCS} ${LIBFT} ${LDFLAGS} -o ${NAME}

${LIBFT}:
	make -C ${LIBFT_DIR}

clean:
	rm -rf ${OBJCS}
	make clean -C ${LIBFT_DIR}

fclean: clean
	rm -rf ${NAME}
	make fclean -C ${LIBFT_DIR}

re:
	make fclean
	make all

.PHONY: all clean fclean re
