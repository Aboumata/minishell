/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:59:03 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/15 14:59:05 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exit_numeric_error(const char *arg)
{
	printf("minishell: exit: %s: numeric argument required\n", arg);
	free_env(g_env);
	exit(2);
}

static void	exit_cleanup_and_exit(long long code)
{
	free_env(g_env);
	exit((unsigned char)code);
}

int	builtin_exit(char **argv)
{
	long long	code;
	char		*endptr;

	printf("exit\n");
	if (!argv[1])
		exit_cleanup_and_exit(0);
	code = strtoll(argv[1], &endptr, 10);
	if (*endptr)
		exit_numeric_error(argv[1]);
	if (argv[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	exit_cleanup_and_exit(code);
	return (0);
}
