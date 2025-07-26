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
#include "minishell_exec.h"
#include "parsing/parsing.h"
#include "pipe_structures.h"

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

int	is_builtin_match(const char *input, const char *cmd, int len)
{
	return (ft_strncmp(input, cmd, len) == 0 && (input[len] == '\0'
			|| input[len] == ' '));
}

void	handle_export_unset(char *input)
{
	char	**args;

	args = mini_shell_split(input);
	if (is_builtin_match(input, "export", 6))
		g_last_status = builtin_export(&g_env, args);
	else
		g_last_status = builtin_unset(args, &g_env);
	free_split(args);
}

static void	handle_input(char *input)
{
	extern char	**environ;

	handle_input_with_pipes(input, environ);
}

static void	set_prompt(char *prompt, char *cwd, char *home)
{
	prompt[0] = '\0';
	if (getcwd(cwd, 4096) != NULL)
	{
		if (home && !ft_strncmp(cwd, home, ft_strlen(home)))
		{
			ft_strlcat(prompt, "~", 4120);
			ft_strlcat(prompt, cwd + ft_strlen(home), 4120);
		}
		else
			ft_strlcat(prompt, cwd, 4120);
	}
	ft_strlcat(prompt, "$ ", 4120);
}

static void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

static int	process_input(char *input)
{
	if (!input)
		return (0);
	if (*input)
	{
		add_history(input);
		handle_input(input);
	}
	free(input);
	return (1);
}

static void	cleanup_and_exit(void)
{
	clear_history();
	free_env(g_env);
	g_env = NULL;
}

int	main(const int argc, char **argv, char *envp[])
{
	char	*input;
	char	cwd[4096];
	char	prompt[4120];
	char	*home;

	(void)argc;
	(void)argv;
	g_env = init_env(envp);
	setup_signals();
	while (1)
	{
		home = getenv("HOME");
		set_prompt(prompt, cwd, home);
		input = readline(prompt);
		if (!process_input(input))
			break ;
	}
	cleanup_and_exit();
	return (0);
}
