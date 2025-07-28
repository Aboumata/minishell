/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor_main.c                               :+:      :+:    :+:   */
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

int	**create_pipes(int count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe failed");
			while (i > 0)
				free(pipes[--i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	free_pipes(int **pipes, int count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < count)
	{
		if (pipes[i])
			free(pipes[i]);
		i++;
	}
	free(pipes);
}

static void	close_all_pipes(int **pipes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

static void	execute_pipeline_child(t_command *cmd, int **pipes, int cmd_index,
		int total_cmds, char **envp)
{
	char	*path;

	if (cmd_index > 0)
		dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < total_cmds - 1)
		dup2(pipes[cmd_index][1], STDOUT_FILENO);
	close_all_pipes(pipes, total_cmds - 1);
	if (cmd->redirections && setup_redirections(cmd->redirections) == -1)
		exit(1);
	if (cmd->is_builtin)
		exit(execute_builtin_in_pipe(cmd, STDIN_FILENO, STDOUT_FILENO));

	path = find_executable(cmd->args[0]);
	if (!path)
	{
		exit(127);
	}
	execve(path, cmd->args, envp);
	perror("execve failed");
	exit(1);
}

int	execute_pipeline(t_pipeline *pipeline, char **envp)
{
	int			**pipes;
	pid_t		*pids;
	t_command	*current;
	int			i;
	int			status;

	if (!pipeline || !pipeline->commands)
		return (-1);
	if (pipeline->cmd_count == 1)
		return (execute_single_command(pipeline->commands, envp));

	pipes = create_pipes(pipeline->cmd_count - 1);
	pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!pipes || !pids)
		return (-1);
	current = pipeline->commands;
	i = 0;
	while (current && i < pipeline->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork failed");
			return (-1);
		}
		if (pids[i] == 0)
		{
			execute_pipeline_child(current, pipes, i, pipeline->cmd_count, envp);
		}
		current = current->next;
		i++;
	}

	close_all_pipes(pipes, pipeline->cmd_count - 1);
	status = wait_for_all_children(pids, pipeline->cmd_count);

	free_pipes(pipes, pipeline->cmd_count - 1);
	free(pids);
	return (status);
}
