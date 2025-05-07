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
void				print_env(t_envs *env);
t_envs				*get_env(t_envs *env, char *name);
void				update_env(t_envs *env, char *name, char *new_value);
t_envs				*create_env(char *env_strs);
t_envs				*init_env(char *envp[]);
void				quick_sort(t_envs **arr, int low, int high);
void				sorted_env(t_envs *env);
#endif
