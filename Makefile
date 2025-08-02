CC = cc
CFLAGS = -g -Wall -Wextra -Werror

SRC = free_env.c minishell.c utils_env_V1.c utils_env_V2.c quick_sort.c path_utils.c\
	  command_finder.c executor.c external_commands.c pipe_parser_utils.c pipe_parser.c\
	  pipe_validator.c pipe_executor_utils.c pipe_executor_child.c pipe_executor_main.c\
	  pipe_integration.c redirection_parser.c redirection_utils.c heredoc_handler.c\
	  redirection_executor.c redirection_integration.c parsing/expander_main.c parsing/expander_quote_handler.c parsing/token_processor_1.c\
	  parsing/token_processor_2.c parsing/utils_export.c parsing/mini_shell_split_1.c parsing/expander.c parsing/mini_shell_split_2.c\
	  parsing/expander_utils.c builtins/cd.c builtins/pwd.c builtins/echo.c builtins/env.c \
	  builtins/exit.c builtins/export.c builtins/unset.c\
	  signals/signals_interactive.c signals/signals_execution.c signals/signals_heredoc.c\

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
