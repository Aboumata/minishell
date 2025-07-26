/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/26 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_exec.h"
#include "pipe_structures.h"

int	is_builtin_command(const char *cmd)
{
	if (!cmd)
		return (0);
	if (safe_strcmp(cmd, "cd") || safe_strcmp(cmd, "pwd"))
		return (1);
	if (safe_strcmp(cmd, "echo") || safe_strcmp(cmd, "env"))
		return (1);
	if (safe_strcmp(cmd, "export") || safe_strcmp(cmd, "unset"))
		return (1);
	if (safe_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

static int	redirect_fds(int input_fd, int output_fd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
		return (-1);
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	return ((saved_stdin << 16) | saved_stdout);
}

static void	restore_fds(int saved_fds)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = saved_fds >> 16;
	saved_stdout = saved_fds & 0xFFFF;
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

static int	exec_builtin_cmd(t_command *cmd)
{
	if (safe_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args));
	if (safe_strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd());
	if (safe_strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	if (safe_strcmp(cmd->args[0], "env"))
	{
		print_env(g_env);
		return (0);
	}
	if (safe_strcmp(cmd->args[0], "export"))
		return (builtin_export(&g_env, cmd->args));
	if (safe_strcmp(cmd->args[0], "unset"))
		return (builtin_unset(cmd->args, &g_env));
	if (safe_strcmp(cmd->args[0], "exit"))
		return (builtin_exit(cmd->args));
	return (127);
}

int	execute_builtin_in_pipe(t_command *cmd, int input_fd, int output_fd)
{
	int	saved_fds;
	int	status;

	saved_fds = redirect_fds(input_fd, output_fd);
	if (saved_fds < 0)
		return (-1);
	status = exec_builtin_cmd(cmd);
	restore_fds(saved_fds);
	return (status);
}
