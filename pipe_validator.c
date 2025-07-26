/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_validator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/26 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipe_structures.h"

void	print_syntax_error(const char *token)
{
	printf("minishell: syntax error near unexpected token `%s'\n", token);
}

int	check_pipe_at_start(char **tokens)
{
	if (safe_strcmp(tokens[0], "|"))
	{
		print_syntax_error("|");
		return (0);
	}
	return (1);
}

int	check_consecutive_pipes(char **tokens, int i)
{
	if (tokens[i + 1] && safe_strcmp(tokens[i + 1], "|"))
	{
		print_syntax_error("|");
		return (0);
	}
	return (1);
}

int	check_pipe_at_end(char **tokens, int i)
{
	if (!tokens[i + 1])
	{
		print_syntax_error("newline");
		return (0);
	}
	return (1);
}

int	validate_pipe_syntax(char **tokens)
{
	int	i;
	int	has_command;

	if (!tokens || !tokens[0])
		return (0);
	if (!check_pipe_at_start(tokens))
		return (0);
	i = 0;
	has_command = 0;
	while (tokens[i])
	{
		if (safe_strcmp(tokens[i], "|"))
		{
			if (!has_command)
			{
				print_syntax_error("|");
				return (0);
			}
			if (!check_consecutive_pipes(tokens, i))
				return (0);
			if (!check_pipe_at_end(tokens, i))
				return (0);
			has_command = 0;
		}
		else
			has_command = 1;
		i++;
	}
	return (1);
}
