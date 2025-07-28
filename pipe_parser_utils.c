/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parser_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/26 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipe_structures.h"

int	count_commands_in_pipeline(char **tokens)
{
	int	count;
	int	i;

	count = 1;
	i = 0;
	while (tokens[i])
	{
		if (safe_strcmp(tokens[i], "|"))
			count++;
		i++;
	}
	return (count);
}

static void	free_split_partial(char **args, int count)
{
	int	i;

	if (!args)
		return;
	i = 0;
	while (i < count)
	{
		if (args[i])
			free(args[i]);
		i++;
	}
	free(args);
}

// Simplify extract_command_args - just copy all tokens for now
char	**extract_command_args(char **tokens, int *start, int end)
{
	char	**args;
	int		count;
	int		i;
	int		j;

	count = end - *start;
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);

	j = 0;
	i = *start;
	while (i < end)
	{
		args[j] = ft_strdup(tokens[i]);
		if (!args[j])
		{
			free_split_partial(args, j);
			return (NULL);
		}
		j++;
		i++;
	}
	args[j] = NULL;
	*start = end + 1; // Skip the pipe token
	return (args);
}

t_command	*create_command(char **args)
{
	t_command	*cmd;
	int			i;
	int			argc;

	if (!args || !args[0])
		return (NULL);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);

	// Count arguments
	argc = 0;
	while (args[argc])
		argc++;

	// Allocate and copy arguments
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}

	i = 0;
	while (i < argc)
	{
		cmd->args[i] = ft_strdup(args[i]);
		if (!cmd->args[i])
		{
			while (i > 0)
				free(cmd->args[--i]);
			free(cmd->args);
			free(cmd);
			return (NULL);
		}
		i++;
	}
	cmd->args[argc] = NULL;

	cmd->is_builtin = is_builtin_command(args[0]);
	cmd->redirections = NULL; // Initialize redirections
	cmd->next = NULL;

	return (cmd);
}

void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->redirections)
		free_redirection(cmd->redirections);

	free(cmd);
}

void	free_pipeline(t_pipeline *pipeline)
{
	t_command	*current;
	t_command	*next;

	if (!pipeline)
		return ;
	current = pipeline->commands;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
	free(pipeline);
}
