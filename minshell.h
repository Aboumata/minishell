/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minshell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:39:14 by aboumata          #+#    #+#             */
/*   Updated: 2025/04/26 12:39:17 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINSHELL_H
# define MINSHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>

typedef struct s_envs
{
	char			*names;
	char			*values;
	struct s_envs	*next;
}					t_envs;

void				free_env(t_envs *env);
void				free_split(char **split);
void				remove_env(t_envs **env, char *name);
void				print_env(t_envs *env);
t_envs				*get_env(t_envs *env, char *name);
void				update_env(t_envs *env, char *name, char *new_value);
t_envs				*create_env(char *env_strs);
t_envs				*init_env(char *envp[]);
#endif
