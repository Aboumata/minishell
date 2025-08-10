/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_integration.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/28 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipe_structures.h"
#include "redirection_structures.h"
#include "minishell_exec.h"

static t_cmd_with_redir	*create_cmd_with_redir(char **args)
{
	t_cmd_with_redir	*cmd;

	cmd = malloc(sizeof(t_cmd_with_redir));
	if (!cmd)
		return (NULL);
	cmd->args = args;
	cmd->redirections = NULL;
	cmd->is_builtin = 0;
	if (args && args[0])
		cmd->is_builtin = is_builtin_command(args[0]);
	return (cmd);
}

static int	execute_builtin_with_redir(t_cmd_with_redir *cmd)
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

static int	exec_cmd_with_redirections(t_cmd_with_redir *cmd, char **envp)
{
	int		saved_stdin;
	int		saved_stdout;
	int		status;
	char	*path;
	pid_t	pid;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);

	if (setup_redirections(cmd->redirections) != 0)
	{
		restore_stdio(saved_stdin, saved_stdout);
		return (1);
	}
	if (cmd->is_builtin)
	{
		status = execute_builtin_with_redir(cmd);
		restore_stdio(saved_stdin, saved_stdout);
		return (status);
	}
	path = find_executable(cmd->args[0]);
	if (!path)
	{
		restore_stdio(saved_stdin, saved_stdout);
		return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		execve(path, cmd->args, envp);
		perror("execve failed");
		exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = 128 + WTERMSIG(status);
		else
			status = 1;
	}
	else
	{
		perror("fork failed");
		status = 1;
	}

	restore_stdio(saved_stdin, saved_stdout);
	free(path);
	return (status);
}

static int	handle_heredoc_only(t_redirection *redirections, char **envp)
{
	t_cmd_with_redir	*cmd;
	char				**default_args;
	int					status;

	default_args = malloc(sizeof(char *) * 2);
	if (!default_args)
		return (1);
	default_args[0] = ft_strdup("cat");
	default_args[1] = NULL;
	if (!default_args[0])
	{
		free(default_args);
		return (1);
	}
	cmd = create_cmd_with_redir(default_args);
	if (!cmd)
	{
		free_split(default_args);
		return (1);
	}
	cmd->redirections = redirections;
	cmd->is_builtin = 0;
	status = exec_cmd_with_redirections(cmd, envp);
	free_cmd_with_redir(cmd);
	return (status);
}

static int	handle_command_with_redirections(char **tokens, char **envp)
{
	t_cmd_with_redir	*cmd;
	char				**clean_args;
	t_redirection		*redirections;
	int					status;

	redirections = parse_redirections(tokens, &clean_args);
	if (!clean_args && redirections)
		return (handle_heredoc_only(redirections, envp));

	if (!clean_args)
	{
		free_redirection(redirections);
		return (1);
	}
	cmd = create_cmd_with_redir(clean_args);
	if (!cmd)
	{
		free_redirection(redirections);
		free_split(clean_args);
		return (1);
	}
	cmd->redirections = redirections;
	status = exec_cmd_with_redirections(cmd, envp);
	free_cmd_with_redir(cmd);
	return (status);
}

int	handle_input_with_redirections(char **tokens, char **envp)
{
	if (!tokens || !tokens[0])
		return (127);
	if (has_redirections(tokens))
		return (handle_command_with_redirections(tokens, envp));
	return (handle_external_command(tokens, envp));
}
