/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                         :+:      :+:   :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:39:14 by aboumata          #+#    #+#             */
/*   Updated: 2025/04/26 12:39:17 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <curses.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <term.h>
# include <unistd.h>
# include <wait.h>

typedef struct s_envs
{
	char			*names;
	char			*values;
	struct s_envs	*next;
}					t_envs;

extern t_envs		*g_env;
extern int g_last_status;

void				free_env(t_envs *env);
bool				safe_strcmp(const char *s1, const char *s2);
void				remove_env(t_envs **env, char *name);
void				print_env(const t_envs *env);
t_envs				*get_env(t_envs *env, const char *name);
void				update_env(t_envs *env, const char *name,
						const char *new_value);
t_envs				*create_env(const char *env_strs);
t_envs				*init_env(char *envp[]);
void				quick_sort(t_envs **arr, const int low, const int high);
void				sorted_env(t_envs *env);
t_envs				*add_env(t_envs *env, const char *names,
						const char *values);
void				free_split(char **result);
char				**mini_shell_split(const char *str);
int					builtin_export(t_envs **env, char *arg[]);
int					builtin_cd(char **args);
int					builtin_pwd(void);
int					builtin_echo(char **argv);
int					builtin_exit(char **argv);
int					builtin_unset(char **argv, t_envs **env);
char				*strip_quotes(const char *value);
t_envs				*copy_env_node(t_envs *node);
t_envs				*copy_env_list(t_envs *env);
int					is_builtin_match(const char *input, const char *cmd, int len);
void				handle_input_with_pipes(char *input, char **environ);
void				handle_export_unset(char *input);

#endif
