
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_executor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/28 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_exec.h"
#include "redirection_structures.h"

int	save_original_fds(void)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		if (saved_stdin != -1)
			close(saved_stdin);
		if (saved_stdout != -1)
			close(saved_stdout);
		return (-1);
	}
	return ((saved_stdin << 16) | saved_stdout);
}

int	restore_original_fds(int saved_stdin, int saved_stdout)
{
	int	result;

	result = 0;
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		result = -1;
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		result = -1;
	close(saved_stdin);
	close(saved_stdout);
	return (result);
}

int	handle_single_redirection(t_redirection *redir)
{
	int	fd;

	if (redir->type == REDIR_IN)
	{
		fd = open(redir->file, O_RDONLY);
		if (fd == -1)
			return (perror(redir->file), -1);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_OUT)
	{
		fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			return (perror(redir->file), -1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_APPEND)
	{
		fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			return (perror(redir->file), -1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_HEREDOC)
	{
		fd = handle_heredoc(redir->delimiter);
		if (fd == -1)
			return (-1);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

int	setup_redirections(t_redirection *redirections)
{
	t_redirection	*current;

	current = redirections;
	while (current)
	{
		if (handle_single_redirection(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}

static int	execute_builtin_redir(t_cmd_with_redir *cmd)
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

int	execute_with_redirections(t_cmd_with_redir *cmd, char **envp)
{
	int	saved_fds;
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (-1);
	saved_fds = save_original_fds();
	if (saved_fds == -1)
		return (1);
	saved_stdin = saved_fds >> 16;
	saved_stdout = saved_fds & 0xFFFF;
	if (setup_redirections(cmd->redirections) == -1)
	{
		restore_original_fds(saved_stdin, saved_stdout);
		return (1);
	}
	if (cmd->is_builtin)
		status = execute_builtin_redir(cmd);
	else
		status = handle_external_command(cmd->args, envp);
	restore_original_fds(saved_stdin, saved_stdout);
	return (status);
}
