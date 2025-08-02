/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:53:15 by aboumata          #+#    #+#             */
/*   Updated: 2025/08/02 10:53:18 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_exec.h"
#include "signals/signals.h"

char	**create_argv(char **tokens)
{
	int		token_count;
	char	**argv;
	int		i;

	if (!tokens)
		return (NULL);
	token_count = 0;
	while (tokens[token_count])
		token_count++;
	argv = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		argv[i] = tokens[i];
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

int	wait_for_child(pid_t pid)
{
	int	status;

	if (pid <= 0)
		return (-1);
	setup_parent_execution_signals();
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid failed");
		restore_interactive_signals();
		return (-1);
	}
	restore_interactive_signals();
	return (handle_child_exit_status(status));
}

int	execute_command(const char *path, char **argv, char **envp)
{
	pid_t	pid;

	if (!path || !argv || !argv[0])
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (-1);
	}
	if (pid == 0)
	{
		setup_child_signals();
		execve(path, argv, envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	return (wait_for_child(pid));
}
