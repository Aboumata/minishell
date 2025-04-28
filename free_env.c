/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:04:59 by aboumata          #+#    #+#             */
/*   Updated: 2025/04/26 16:05:01 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minshell.h"

void	free_env(t_envs *env)
{
	t_envs	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->names);
		free(env->values);
		free(env);
		env = tmp;
	}
}
