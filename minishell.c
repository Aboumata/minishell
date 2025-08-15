/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:08:49 by aboumata          #+#    #+#             */
/*   Updated: 2025/08/02 15:08:50 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parsing.h"
#include "signals/signals.h"

t_envs		*g_env = NULL;
int			g_last_status = 0;

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

int	is_empty_after_expansion(const char *input)
{
	char	**tokens;
	char	**expanded_tokens;
	int		is_empty;

	if (!input || !*input)
		return (1);

	tokens = mini_shell_split(input);
	if (!tokens)
		return (1);

	expanded_tokens = expand_tokens(tokens, g_env, g_last_status);
	free_tokens(tokens);

	if (!expanded_tokens)
	{
		is_empty = 1;
	}
	else
	{
		is_empty = 0;
		free_tokens(expanded_tokens);
	}

	return (is_empty);
}

static void	handle_input(char *input)
{
	extern char	**environ;

	if (check_signal_flag())
	{
		g_last_status = 130;
		reset_signal_flag();
		return ;
	}

	if (is_empty_after_expansion(input))
	{
		g_last_status = 0;
		return ;
	}

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

static int	process_input(char *input)
{
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (0);
	}
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
	setup_interactive_signals();
	while (1)
	{
		reset_signal_flag();
		home = getenv("HOME");
		set_prompt(prompt, cwd, home);
		input = readline(prompt);
		if (!process_input(input))
			break ;
	}
	cleanup_and_exit();
	return (g_last_status);
}
