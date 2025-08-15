/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_validator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/28 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection_structures.h"

static void	write_str(int fd, const char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(fd, str, len);
}

void	print_syntax_error(char *token)
{
	write_str(STDERR_FILENO, "minishell: syntax error near unexpected token `");
	write_str(STDERR_FILENO, token);
	write_str(STDERR_FILENO, "'\n");
}

static int	is_redirection_operator(const char *token)
{
	return (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0 || strcmp(token,
			"<") == 0 || strcmp(token, "<<") == 0);
}

static int	validate_redirection_syntax(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (is_redirection_operator(tokens[i]))
		{
			// Check for redirection without filename/delimiter
			if (!tokens[i + 1])
			{
				print_syntax_error("newline");
				return (0);
			}
			// Check for redirection followed by another operator
			if (is_redirection_operator(tokens[i + 1]) || strcmp(tokens[i + 1],
					"|") == 0)
			{
				if (strcmp(tokens[i + 1], "|") == 0)
					print_syntax_error("|");
				else
					print_syntax_error(tokens[i + 1]);
				return (0);
			}
			i += 2; // Skip redirection and its target
		}
		else if (strcmp(tokens[i], "|") == 0)
		{
			// Check for pipe after redirection without filename
			if (i > 0 && is_redirection_operator(tokens[i - 1]))
			{
				print_syntax_error("|");
				return (0);
			}
			i++;
		}
		else
			i++;
	}
	return (1);
}

static int	validate_triple_redirect(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		// Check for ">>>"
		if (strcmp(tokens[i], ">>") == 0 && tokens[i + 1] && strcmp(tokens[i
				+ 1], ">") == 0)
		{
			print_syntax_error(">");
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_pipe_at_start(char **tokens)
{
	if (tokens[0] && strcmp(tokens[0], "|") == 0)
	{
		print_syntax_error("|");
		return (0);
	}
	return (1);
}

int	check_consecutive_pipes(char **tokens, int i)
{
	if (strcmp(tokens[i], "|") == 0)
	{
		// Check for consecutive pipes
		if (tokens[i + 1] && strcmp(tokens[i + 1], "|") == 0)
		{
			print_syntax_error("|");
			return (0);
		}
		// Check for pipe followed by redirection
		if (tokens[i + 1] && is_redirection_operator(tokens[i + 1]))
		{
			// For "echo hi | >" -> error should be "newline"
			// For "echo hi | > >>" -> error should be ">>"
			// For "echo hi | < |" -> error should be "|"
			if (!tokens[i + 2])
			{
				print_syntax_error("newline");
				return (0);
			}
			else if (is_redirection_operator(tokens[i + 2]))
			{
				print_syntax_error(tokens[i + 2]);
				return (0);
			}
			else if (strcmp(tokens[i + 2], "|") == 0)
			{
				print_syntax_error("|");
				return (0);
			}
		}
	}
	return (1);
}

int	check_pipe_at_end(char **tokens, int i)
{
	if (strcmp(tokens[i], "|") == 0 && !tokens[i + 1])
	{
		print_syntax_error("newline");
		return (0);
	}
	return (1);
}

int	validate_pipe_syntax(char **tokens)
{
	int	i;

	if (!tokens || !tokens[0])
		return (1);
	// Check for triple redirect first
	if (!validate_triple_redirect(tokens))
		return (0);
	// Check for redirection syntax errors
	if (!validate_redirection_syntax(tokens))
		return (0);
	// Check pipe at start
	if (!check_pipe_at_start(tokens))
		return (0);
	// Check for consecutive pipes and pipe at end
	i = 0;
	while (tokens[i])
	{
		if (!check_consecutive_pipes(tokens, i))
			return (0);
		if (!check_pipe_at_end(tokens, i))
			return (0);
		i++;
	}
	return (1);
}
