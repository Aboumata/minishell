/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_integration.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/26 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"
# include "parsing/parsing.h"
# include "minishell_exec.h"
# include "pipe_structures.h"

static int	handle_pipeline_execution(char **args, char **environ)
{
	t_pipeline	*pipeline;
	int			status;

	if (!validate_pipe_syntax(args))
	{
		g_last_status = 2;
		return (2);
	}
	pipeline = parse_pipeline(args);
	if (!pipeline)
	{
		g_last_status = 1;
		return (1);
	}
	status = execute_pipeline(pipeline, environ);
	free_pipeline(pipeline);
	g_last_status = status;
	return (status);
}

static int	handle_single_env_command(char **args)
{
	if (args && args[0] && safe_strcmp(args[0], "env"))
	{
		print_env(g_env);
		g_last_status = 0;
		return (1);  // Command was handled
	}
	return (0);  // Command was not handled
}

static int	handle_single_builtin(char **args)
{
	if (!args || !args[0])
		return (0);

	if (safe_strcmp(args[0], "cd"))
	{
		g_last_status = builtin_cd(args);
		return (1);
	}
	else if (safe_strcmp(args[0], "pwd"))
	{
		g_last_status = builtin_pwd();
		return (1);
	}
	else if (safe_strcmp(args[0], "echo"))
	{
		g_last_status = builtin_echo(args);
		return (1);
	}
	else if (safe_strcmp(args[0], "exit"))
	{
		int exit_status = builtin_exit(args);
		g_last_status = exit_status;
		return (1);
	}
	return (0);
}

static void	handle_single_command(char **args, char **environ)
{
	if (handle_single_env_command(args))
		return;
	if (handle_single_builtin(args))
		return;
	g_last_status = handle_external_command(args, environ);
}

void	handle_input_with_pipes(char *input, char **environ)
{
	char	**args;

	// Handle export/unset specially since they affect the global environment
	if (is_builtin_match(input, "export", 6) || is_builtin_match(input, "unset", 5))
	{
		handle_export_unset(input);
		return ;
	}

	// Parse the command line with proper expansion and quote stripping
	args = parse_command_line(input, g_env, g_last_status);
	if (!args || !args[0])
	{
		g_last_status = 127;
		if (args)
			free_tokens(args);
		return ;
	}

	if (contains_pipe(args))
		handle_pipeline_execution(args, environ);
	else
		handle_single_command(args, environ);

	free_tokens(args);
}
