/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor_child.c                              :+:      :+:    :+:   */
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

int	execute_single_command(t_command *cmd, char **envp)
{
	char	*path;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (-1);
	if (cmd->is_builtin)
		return (execute_builtin_in_pipe(cmd, STDIN_FILENO, STDOUT_FILENO));
	path = find_executable(cmd->args[0]);
	if (!path)
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		return (127);
	}
	status = execute_command(path, cmd->args, envp);
	free(path);
	return (status);
}

int	wait_for_all_children(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	last_status;

	last_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			if (waitpid(pids[i], &status, 0) == -1)
			{
				perror("waitpid failed");
				return (-1);
			}
			if (i == cmd_count - 1)
			{
				if (WIFEXITED(status))
					last_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					last_status = 128 + WTERMSIG(status);
			}
		}
		i++;
	}
	return (last_status);
}

static void	setup_child_fds(int input_fd, int output_fd)
{
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
}

static void	execute_child_process(t_command *cmd, int input_fd, int output_fd,
		char **envp)
{
	char	*path;

	setup_child_fds(input_fd, output_fd);

	if (cmd->is_builtin)
	{
		int status = execute_builtin_in_pipe(cmd, STDIN_FILENO, STDOUT_FILENO);
		exit(status);
	}

	path = find_executable(cmd->args[0]);
	if (!path)
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	execve(path, cmd->args, envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

pid_t	fork_and_execute(t_command *cmd, int input_fd, int output_fd,
		char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (-1);
	}
	if (pid == 0)
		execute_child_process(cmd, input_fd, output_fd, envp);
	return (pid);
}
