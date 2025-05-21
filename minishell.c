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

#include "expander.h"
#include "minishell.h"

t_envs	*g_env = NULL;
int		g_last_status = 0;

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

static int	is_builtin_match(const char *input, const char *cmd, int len)
{
	return (ft_strncmp(input, cmd, len) == 0 && (input[len] == '\0'
			|| input[len] == ' '));
}

static void	handle_export_unset(char *input)
{
	char	**args;

	args = mini_shell_split(input);
	if (is_builtin_match(input, "export", 6))
		g_last_status = builtin_export(&g_env, args);
	else
		g_last_status = builtin_unset(args, &g_env);
	free_split(args);
}

static void	handle_other_builtins(char *expanded)
{
	char	**args;
	int		exit_status;

	if (safe_strcmp(expanded, "env"))
	{
		print_env(g_env);
		g_last_status = 0;
	}
	else if (is_builtin_match(expanded, "cd", 2))
	{
		args = mini_shell_split(expanded);
		g_last_status = builtin_cd(args[1]);
		free_split(args);
	}
	else if (is_builtin_match(expanded, "pwd", 3))
	{
		g_last_status = builtin_pwd();
	}
	else if (is_builtin_match(expanded, "echo", 4))
	{
		args = mini_shell_split(expanded);
		g_last_status = builtin_echo(args);
		free_split(args);
	}
	else if (is_builtin_match(expanded, "clear", 5))
	{
		builtin_clear();
	}
	else if (is_builtin_match(expanded, "exit", 4))
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
}

static void	handle_input(char *input)
{
	char	*expanded;

	if (is_builtin_match(input, "export", 6) || is_builtin_match(input, "unset", 5))
	{
		handle_export_unset(input);
		return ;
	}
	expanded = expand_variables(input, g_env, g_last_status);
	handle_other_builtins(expanded);
	free(expanded);
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
		{
			ft_strlcat(prompt, cwd, 4120);
		}
	}
	ft_strlcat(prompt, "$ ", 4120);
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
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	while (1)
	{
		home = getenv("HOME");
		set_prompt(prompt, cwd, home);
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
