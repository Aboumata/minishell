/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:08:49 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/03 15:08:50 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envs		*g_env = NULL;

static void	handle_input(char *input)
{
	char	**args;

	if (safe_strcmp(input, "env"))
		print_env(g_env);
	else if (ft_strncmp(input, "export", 6) == 0 && (input[6] == '\0'
			|| input[6] == ' '))
	{
		args = mini_shell_split(input);
		into_export(&g_env, args);
		free_split(args);
	}
}

int	main(const int argc, char **argv, char *envp[])
{
	char	*input;

	g_env = init_env(envp);
	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			handle_input(input);
		}
		free(input);
	}
	clear_history();
	free_env(g_env);
	return (0);
}
