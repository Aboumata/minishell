/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_integration.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/28 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"
# include "parsing/parsing.h"
# include "minishell_exec.h"
# include "pipe_structures.h"
# include "redirection_structures.h"

static void	write_str(int fd, const char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(fd, str, len);
}

static void	print_raw_syntax_error(char *token)
{
	write_str(STDERR_FILENO, "minishell: syntax error near unexpected token `");
	write_str(STDERR_FILENO, token);
	write_str(STDERR_FILENO, "'\n");
}

static int	is_redirect_start(const char *input, int i)
{
	if (input[i] == '>' || input[i] == '<')
		return (1);
	return (0);
}

static int	get_redirect_length(const char *input, int i)
{
	if (input[i] == '>' && input[i + 1] == '>')
		return (2);
	if (input[i] == '<' && input[i + 1] == '<')
		return (2);
	if (input[i] == '>' || input[i] == '<')
		return (1);
	return (0);
}

static int	is_and_operator(const char *input, int i)
{
	if (input[i] == '&' && input[i + 1] == '&')
		return (1);
	return (0);
}

static int	is_semicolon(const char *input, int i)
{
	if (input[i] == ';')
		return (1);
	return (0);
}

static int	skip_whitespace(const char *input, int start)
{
	int i = start;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	return (i);
}

static int	validate_and_operators(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;

	i = skip_whitespace(input, 0);

	if (is_and_operator(input, i))
	{
		print_raw_syntax_error("&&");
		return (0);
	}

	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		else if (!quote && is_and_operator(input, i))
		{
			int j = skip_whitespace(input, i + 2);

			if (!input[j])
			{
				print_raw_syntax_error("newline");
				return (0);
			}

			if (is_and_operator(input, j))
			{
				print_raw_syntax_error("&&");
				return (0);
			}

			if (input[j] == '|')
			{
				print_raw_syntax_error("|");
				return (0);
			}

			i++;
		}
		i++;
	}
	return (1);
}

static int	validate_semicolon_operators(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;

	i = skip_whitespace(input, 0);
	if (is_semicolon(input, i))
	{
		print_raw_syntax_error(";");
		return (0);
	}

	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		else if (!quote && is_semicolon(input, i))
		{
			int j = skip_whitespace(input, i + 1);
			if (is_semicolon(input, j))
			{
				print_raw_syntax_error(";");
				return (0);
			}
			if (input[j] == '|')
			{
				print_raw_syntax_error("|");
				return (0);
			}
			if (is_and_operator(input, j))
			{
				print_raw_syntax_error("&&");
				return (0);
			}
		}
		i++;
	}
	return (1);
}

static int	validate_raw_syntax(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	if (!validate_and_operators(input))
		return (0);
	if (!validate_semicolon_operators(input))
		return (0);
	i = skip_whitespace(input, 0);
	if (input[i] == '|')
	{
		print_raw_syntax_error("|");
		return (0);
	}
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		else if (!quote)
		{
			if (is_redirect_start(input, i))
			{
				int redirect_len = get_redirect_length(input, i);
				if (redirect_len == 2 && input[i] == '>' && input[i + 2] == '>')
				{
					print_raw_syntax_error(">");
					return (0);
				}
				int j = skip_whitespace(input, i + redirect_len);

				if (!input[j])
				{
					print_raw_syntax_error("newline");
					return (0);
				}

				if (is_redirect_start(input, j))
				{
					int next_len = get_redirect_length(input, j);
					char next_op[3] = {0};
					int k;
					for (k = 0; k < next_len && k < 2; k++)
						next_op[k] = input[j + k];
					print_raw_syntax_error(next_op);
					return (0);
				}

				if (input[j] == '|')
				{
					print_raw_syntax_error("|");
					return (0);
				}

				if (is_and_operator(input, j))
				{
					print_raw_syntax_error("&&");
					return (0);
				}


				if (is_semicolon(input, j))
				{
					print_raw_syntax_error(";");
					return (0);
				}

				i += redirect_len - 1;
			}
			else if (input[i] == '|')
			{
				int j = skip_whitespace(input, i + 1);

				if (!input[j])
				{
					print_raw_syntax_error("newline");
					return (0);
				}

				if (input[j] == '|')
				{
					print_raw_syntax_error("|");
					return (0);
				}

				if (is_and_operator(input, j))
				{
					print_raw_syntax_error("&&");
					return (0);
				}

				if (is_semicolon(input, j))
				{
					print_raw_syntax_error(";");
					return (0);
				}
			}
		}
		i++;
	}
	return (1);
}

static int	handle_pipeline_execution(char **args, char **environ)
{
	t_pipeline	*pipeline;
	int			status;

	if (!validate_pipe_syntax(args))
	{
		g_last_status = 2;
		return (2);
	}

	pipeline = parse_pipeline(args);
	if (!pipeline)
	{
		g_last_status = 1;
		return (1);
	}
	status = execute_pipeline(pipeline, environ);
	free_pipeline(pipeline);
	g_last_status = status;
	return (status);
}

static int	handle_single_env_command(char **args)
{
	if (args && args[0] && safe_strcmp(args[0], "env"))
	{
		print_env(g_env);
		g_last_status = 0;
		return (1);
	}
	return (0);
}

static int	handle_single_builtin(char **args)
{
	if (!args || !args[0])
		return (0);

	if (safe_strcmp(args[0], "cd"))
	{
		g_last_status = builtin_cd(args);
		return (1);
	}
	else if (safe_strcmp(args[0], "pwd"))
	{
		g_last_status = builtin_pwd();
		return (1);
	}
	else if (safe_strcmp(args[0], "echo"))
	{
		g_last_status = builtin_echo(args);
		return (1);
	}
	else if (safe_strcmp(args[0], "export"))
	{
		g_last_status = builtin_export(&g_env, args);
		return (1);
	}
	else if (safe_strcmp(args[0], "unset"))
	{
		g_last_status = builtin_unset(args, &g_env);
		return (1);
	}
	else if (safe_strcmp(args[0], "exit"))
	{
		int exit_status = builtin_exit(args);
		g_last_status = exit_status;
		return (1);
	}
	return (0);
}

static void	handle_single_command(char **args, char **environ)
{
	if (handle_single_env_command(args))
		return;
	if (has_redirections(args))
	{
		g_last_status = handle_input_with_redirections(args, environ);
		return;
	}
	if (handle_single_builtin(args))
		return;

	g_last_status = handle_external_command(args, environ);
}

static int	contains_pipe_in_input(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		else if (!quote && input[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

void	handle_input_with_pipes(char *input, char **environ)
{
	char	**args;

	if (!validate_raw_syntax(input))
	{
		g_last_status = 2;
		return;
	}
	if (!contains_pipe_in_input(input) && (is_builtin_match(input, "export", 6) || is_builtin_match(input, "unset", 5)))
	{
		handle_export_unset(input);
		return ;
	}

	args = parse_command_line(input, g_env, g_last_status);
	if (!args || !args[0])
	{
		g_last_status = 127;
		if (args)
			free_tokens(args);
		return ;
	}

	if (contains_pipe(args))
		handle_pipeline_execution(args, environ);
	else
		handle_single_command(args, environ);
	free_tokens(args);
}
