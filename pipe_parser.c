/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/26 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipe_structures.h"

static t_pipeline	*init_pipeline(char **tokens)
{
	t_pipeline	*pipeline;

	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (NULL);
	pipeline->cmd_count = count_commands_in_pipeline(tokens);
	pipeline->commands = NULL;
	return (pipeline);
}

static int	add_command_to_pipeline(t_pipeline *pipeline, char **args)
{
	t_command	*new_cmd;
	t_command	*current;

	new_cmd = create_command(args);
	if (!new_cmd)
		return (0);
	if (!pipeline->commands)
	{
		pipeline->commands = new_cmd;
		return (1);
	}
	current = pipeline->commands;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
	return (1);
}

static int	parse_commands(t_pipeline *pipeline, char **tokens)
{
	int		start;
	int		i;
	char	**args;

	start = 0;
	i = 0;
	while (tokens[i])
	{
		if (safe_strcmp(tokens[i], "|") || !tokens[i + 1])
		{
			if (safe_strcmp(tokens[i], "|"))
				args = extract_command_args(tokens, &start, i);
			else
				args = extract_command_args(tokens, &start, i + 1);
			if (!args || !add_command_to_pipeline(pipeline, args))
				return (0);
			if (!tokens[i + 1])
				break ;
		}
		i++;
	}
	return (1);
}

t_pipeline	*parse_pipeline(char **tokens)
{
	t_pipeline	*pipeline;

	if (!tokens || !tokens[0])
		return (NULL);
	pipeline = init_pipeline(tokens);
	if (!pipeline)
		return (NULL);
	if (!parse_commands(pipeline, tokens))
	{
		free_pipeline(pipeline);
		return (NULL);
	}
	return (pipeline);
}

int	contains_pipe(char **tokens)
{
	int	i;

	if (!tokens)
		return (0);
	i = 0;
	while (tokens[i])
	{
		if (safe_strcmp(tokens[i], "|"))
			return (1);
		i++;
	}
	return (0);
}
