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
#include "expander.h"

t_envs		*g_env = NULL;
int			g_last_status = 0;

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
	char	*expanded = NULL;
	int		exit_status;
	char	**args = NULL;

	if (ft_strncmp(input, "export", 6) == 0 && (input[6] == '\0' || input[6] == ' '))
	{
		args = mini_shell_split(input);
		g_last_status = builtin_export(&g_env, args);
		free_split(args);
	}
	else if (ft_strncmp(input, "unset", 5) == 0 && (input[5] == '\0' || input[5] == ' '))
	{
		args = mini_shell_split(input);
		g_last_status = builtin_unset(args, &g_env);
		free_split(args);
	}
	else
	{
		expanded = expand_variables(input, g_env, g_last_status);
		if (safe_strcmp(expanded, "env"))
		{
			print_env(g_env);
			g_last_status = 0;
		}
		else if (ft_strncmp(expanded, "cd", 2) == 0 && (expanded[2] == '\0'
				|| expanded[2] == ' '))
		{
			args = mini_shell_split(expanded);
			g_last_status = builtin_cd(args[1]);
			free_split(args);
		}
		else if (ft_strncmp(expanded, "pwd", 3) == 0 && (expanded[3] == '\0'
				|| expanded[3] == ' '))
		{
			g_last_status = builtin_pwd();
		}
		else if (ft_strncmp(expanded, "echo", 4) == 0 && (expanded[4] == '\0'
				|| expanded[4] == ' '))
		{
			args = mini_shell_split(expanded);
			g_last_status = builtin_echo(args);
			free_split(args);
		}
		else if (ft_strncmp(expanded, "clear", 5) == 0 && (expanded[5] == '\0'
			|| expanded[5] == ' '))
		{
			builtin_clear();
		}
		else if (ft_strncmp(expanded, "exit", 4) == 0 && (expanded[4] == '\0'
			|| expanded[4] == ' '))
		{
			args = mini_shell_split(expanded);
			exit_status = builtin_exit(args);
			free_split(args);
			free(expanded);
			clear_history();
			free_env(g_env);
			g_env = NULL;
			exit(exit_status);
		}

		else
			g_last_status = 127;
		free(expanded);
	}
}

int	main(const int argc, char **argv, char *envp[])
{
	char	*input;
	char	cwd[4096];
	char	prompt[4120];
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
	g_env = NULL;
	return (0);
}
