/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:20:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/04/26 16:20:02 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envs	*g_env = NULL;

int	main(int argc, char **argv, char *envp[])
{
	g_env = init_env(envp);
	char *input;

	(void)argc;
	(void)argv;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break;
		if (*input)
		{
			add_history(input);
			//parsing need
		}
			free(input);
	}
	free_env(g_env);
	return (0);
}
