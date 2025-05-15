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

#include "includes/minishell.h"

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

void	free_split(char **result)
{
	int	i;

	if (!result)
		return ;
	i = 0;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
}
