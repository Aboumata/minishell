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

char	**extract_command_args(char **tokens, int *start, int end)
{
	char	**args;
	int		arg_count;
	int		i;
	int		j;

	arg_count = end - *start;
	if (arg_count <= 0)
		return (NULL);
	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	i = *start;
	j = 0;
	while (i < end && j < arg_count)
	{
		args[j] = ft_strdup(tokens[i]);
		if (!args[j])
		{
			while (j > 0)
				free(args[--j]);
			return (free(args), NULL);
		}
		i++;
		j++;
	}
	args[j] = NULL;
	*start = end + 1;
	return (args);
}

t_command	*create_command(char **args)
{
	t_command	*cmd;

	if (!args || !args[0])
		return (NULL);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = args;
	cmd->is_builtin = is_builtin_command(args[0]);
	cmd->next = NULL;
	return (cmd);
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		free_split(cmd->args);
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
