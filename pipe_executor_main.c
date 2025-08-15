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

static int	setup_redirections_child(t_redirection *redirections)
{
	t_redirection	*current;
	int				fd;

	current = redirections;
	while (current)
	{
		if (current->type == REDIR_IN)
		{
			fd = open(current->file, O_RDONLY);
			if (fd == -1)
			{
				perror(current->file);
				return (-1);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2 failed");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		else if (current->type == REDIR_OUT)
		{
			fd = open(current->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(current->file);
				return (-1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 failed");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		else if (current->type == REDIR_APPEND)
		{
			fd = open(current->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(current->file);
				return (-1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 failed");
				close(fd);
				return (-1);
			}
			close(fd);
		}
		else if (current->type == REDIR_HEREDOC)
		{
			// Heredoc FD should already be prepared in parent process
			if (current->heredoc_fd != -1)
			{
				if (dup2(current->heredoc_fd, STDIN_FILENO) == -1)
				{
					perror("dup2 failed");
					close(current->heredoc_fd);
					return (-1);
				}
				close(current->heredoc_fd);
			}
		}
		current = current->next;
	}
	return (0);
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
	if (cmd->redirections && setup_redirections_child(cmd->redirections) == -1)
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

static int	prepare_heredocs_in_pipeline(t_pipeline *pipeline)
{
	t_command	*current;

	current = pipeline->commands;
	while (current)
	{
		// Process all heredocs for this command at once
		if (process_all_heredocs(current->redirections) != 0)
			return (-1);
		current = current->next;
	}
	return (0);
}

static void	cleanup_heredocs_in_pipeline(t_pipeline *pipeline)
{
	t_command		*current;
	t_redirection	*redir;

	current = pipeline->commands;
	while (current)
	{
		redir = current->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC && redir->heredoc_fd != -1)
			{
				close(redir->heredoc_fd);
				redir->heredoc_fd = -1;
			}
			redir = redir->next;
		}
		current = current->next;
	}
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
	// Handle heredocs BEFORE forking any child processes
	if (prepare_heredocs_in_pipeline(pipeline) == -1)
		return (-1);
	pipes = create_pipes(pipeline->cmd_count - 1);
	pids = malloc(sizeof(pid_t) * pipeline->cmd_count);
	if (!pipes || !pids)
	{
		cleanup_heredocs_in_pipeline(pipeline);
		return (-1);
	}
	current = pipeline->commands;
	i = 0;
	while (current && i < pipeline->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork failed");
			cleanup_heredocs_in_pipeline(pipeline);
			return (-1);
		}
		if (pids[i] == 0)
		{
			execute_pipeline_child(current, pipes, i, pipeline->cmd_count,
				envp);
		}
		current = current->next;
		i++;
	}
	close_all_pipes(pipes, pipeline->cmd_count - 1);
	status = wait_for_all_children(pids, pipeline->cmd_count);
	cleanup_heredocs_in_pipeline(pipeline);
	free_pipes(pipes, pipeline->cmd_count - 1);
	free(pids);
	return (status);
}
