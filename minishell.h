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
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_envs
{
	char			*names;
	char			*values;
	struct s_envs	*next;
}					t_envs;

extern t_envs		*g_env;

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
void				into_export(t_envs **env, char *arg[]);
void				free_split(char **result);
char				**mini_shell_split(const char *str);
void				builtin_cd (char *path);

#endif
