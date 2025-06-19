CC = cc
CFLAGS = -g -Wall -Wextra -Werror

SRC = free_env.c minishell.c utils_env_V1.c utils_env_V2.c quick_sort.c path_utils.c\
	  command_finder.c\
	  parsing/utils_export.c parsing/mini_shell_split.c parsing/expander.c\
	  parsing/expander_utils.c builtins/cd.c builtins/pwd.c builtins/echo.c builtins/env.c \
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
