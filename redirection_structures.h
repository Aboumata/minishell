/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_structures.h                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/28 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_STRUCTURES_H
# define REDIRECTION_STRUCTURES_H

# include "minishell.h"

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redirection
{
	t_redir_type		type;
	char				*file;
	char				*delimiter;
	int					heredoc_fd;
	struct s_redirection *next;
}	t_redirection;


typedef struct s_cmd_with_redir
{
	char			**args;
	char			**clean_args;
	t_redirection	*redirections;
	int				is_builtin;
}	t_cmd_with_redir;

/* redirection_parser.c */
int				has_redirections(char **tokens);
t_redirection	*parse_redirections(char **tokens, char ***clean_args);
t_redir_type	get_redirection_type(const char *token);
int				is_redirection_token(const char *token);
t_redirection	*create_redirection(t_redir_type type, const char *target);

/* redirection_executor.c */
int				execute_with_redirections(t_cmd_with_redir *cmd, char **envp);
int				setup_redirections(t_redirection *redirections);
int				restore_original_fds(int saved_stdin, int saved_stdout);
int				handle_single_redirection(t_redirection *redir);
int				save_original_fds(void);

/* heredoc_handler.c */
int				handle_heredoc(const char *delimiter);
char			*generate_temp_filename(void);
int				write_heredoc_content(int fd, const char *delimiter, int should_expand);
int				create_heredoc_file(const char *delimiter);
void			cleanup_heredoc_file(const char *filename);

/* redirection_utils.c */
void			free_redirection(t_redirection *redir);
void			free_cmd_with_redir(t_cmd_with_redir *cmd);
int				count_non_redir_tokens(char **tokens);
char			**extract_clean_args(char **tokens);
t_redirection	*add_redirection_to_list(t_redirection *head, t_redirection *new_redir);
void			restore_stdio(int saved_stdin, int saved_stdout);

#endif
