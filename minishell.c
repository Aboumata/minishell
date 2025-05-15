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

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

static void	handle_input(char *input)
{
	char	**args;

	if (safe_strcmp(input, "env"))
		print_env(g_env);
	else if (ft_strncmp(input, "export", 6) == 0 && (input[6] == '\0'
			|| input[6] == ' '))
	{
		args = mini_shell_split(input);
		builtin_export(&g_env, args);
		free_split(args);
	}
	else if (ft_strncmp(input, "cd", 2) == 0 && (input[2] == '\0'
			|| input[2] == ' '))
	{
		args = split_by_spaces(input);
		builtin_cd(args[1]);
		free_split(args);
	}
	else if (ft_strncmp(input, "pwd", 3) == 0 && (input[3] == '\0'
			|| input[3] == ' '))
		builtin_pwd();
	else if (ft_strncmp(input, "echo", 4) == 0 && (input[4] == '\0'
			|| input[4] == ' '))
	{
		args = mini_shell_split(input);
		builtin_echo(args);
		free_split(args);
	}
	else if (ft_strncmp(input, "exit", 4) == 0 && (input[4] == '\0'
			|| input[4] == ' '))
		builtin_exit();
	else if (strncmp(input, "unset", 5) == 0 && (input[5] == '\0'
			|| input[5] == ' '))
		builtin_unset(input, g_env);
	}

int	main(const int argc, char **argv, char *envp[])
{
	char	*input;
		char cwd[4096];
		char prompt[4120];
	char	*home;

	g_env = init_env(envp);
	(void)argc;
	(void)argv;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	while (1)
	{
		home = getenv("HOME");
		prompt[0] = '\0';
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			if (home && !ft_strncmp(cwd, home, ft_strlen(home)))
			{
				ft_strlcat(prompt, "~", sizeof(prompt));
				ft_strlcat(prompt, cwd + ft_strlen(home), sizeof(prompt));
			}
			else
				ft_strlcat(prompt, cwd, sizeof(prompt));
		}
		ft_strlcat(prompt, "$ ", sizeof(prompt));
		input = readline(prompt);
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
