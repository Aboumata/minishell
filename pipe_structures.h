/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_structures.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/26 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_STRUCTURES_H
# define PIPE_STRUCTURES_H

# include "minishell.h"

typedef struct s_command
{
	char				**args;
	int					is_builtin;
	struct s_command	*next;
}						t_command;

typedef struct s_pipeline
{
	t_command			*commands;
	int					cmd_count;
}						t_pipeline;

/* pipe_parser_utils.c */
int			count_commands_in_pipeline(char **tokens);
char		**extract_command_args(char **tokens, int *start, int end);
t_command	*create_command(char **args);
void		free_command(t_command *cmd);
void		free_pipeline(t_pipeline *pipeline);

/* pipe_parser.c */
t_pipeline	*parse_pipeline(char **tokens);
int			contains_pipe(char **tokens);
int			validate_pipe_syntax(char **tokens);
void		print_syntax_error(const char *token);
int			check_pipe_position(char **tokens, int i);

/* pipe_executor_utils.c */
int			is_builtin_command(const char *cmd);
int			execute_builtin_in_pipe(t_command *cmd, int in_fd, int out_fd);
int			execute_single_command(t_command *cmd, char **envp);
int			wait_for_all_children(pid_t *pids, int cmd_count);
pid_t		fork_and_execute(t_command *cmd, int in_fd, int out_fd, char **envp);

/* pipe_executor.c */
int			execute_pipeline(t_pipeline *pipeline, char **envp);
int			**create_pipes(int count);
void		free_pipes(int **pipes, int count);
void		setup_pipe_fds(int i, int count, int **pipes, int *in_fd, int *out_fd);
void		close_pipe_fds(int i, int count, int **pipes);

#endif
