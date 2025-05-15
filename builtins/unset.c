/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 11:25:50 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/15 11:25:52 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_unset(char **argv, t_envs **env)
{
	int	i;

	(void)env;
	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '-')
		{
			i++;
			continue ;
		}
		remove_env(&g_env, argv[i]);
		i++;
	}
	return (0);
}
