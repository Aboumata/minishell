/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 19:03:27 by aboumata          #+#    #+#             */
/*   Updated: 2025/06/20 19:03:32 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_exec.h"

void	cleanup_execution(char *path, char **argv)
{
	if (path)
		free(path);
	if (argv)
		free(argv);
}

int	handle_special_cases(char *command, char *path)
{
	if (safe_strcmp(path, "IS_DIRECTORY"))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		return (126);
	}
	if (safe_strcmp(path, "PERMISSION_DENIED"))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}
	if (safe_strcmp(path, "NO_SUCH_FILE"))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	return (0);
}

int	handle_dot_command(char **tokens)
{
	if (safe_strcmp(tokens[0], "."))
	{
		if (!tokens[1])
		{
			printf("minishell: .: filename argument required\n");
			printf(".: usage: . filename [arguments]\n");
			return (2);
		}
	}
	return (0);
}

int	handle_external_command(char **tokens, char **envp)
{
	char	*path;
	char	**argv;
	int		status;
	int		special_status;

	if (!tokens || !tokens[0])
		return (-1);
	if (!tokens[0][0])
		return (0);
	special_status = handle_dot_command(tokens);
	if (special_status != 0)
		return (special_status);
	path = find_executable(tokens[0]);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tokens[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		return (127);
	}
	special_status = handle_special_cases(tokens[0], path);
	if (special_status != 0)
	{
		free(path);
		return (special_status);
	}
	argv = create_argv(tokens);
	if (!argv)
	{
		free(path);
		return (-1);
	}
	status = execute_command(path, argv, envp);
	cleanup_execution(path, argv);
	return (status);
}
