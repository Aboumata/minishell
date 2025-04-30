CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = env.c free_env.c main.c utils.c
OBJCS = ${SRC:.c=.o}

NAME = minishell
LIBFT_DIR = libft
LIBFT = ${LIBFT_DIR}/libft.a

all: ${NAME}

${NAME}: ${OBJCS} ${LIBFT}
	${CC} ${CFLAGS} ${OBJCS} ${LIBFT} -o ${NAME}

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
