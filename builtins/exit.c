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

int builtin_exit(char **argv)
{
	long long	code;
	char		*endptr;

	printf("exit\n");
	if (!argv[1])
	{
		free_env(g_env);
		exit(0);
	}
	code = strtoll(argv[1], &endptr, 10);
	if (*endptr != '\0')
	{
		printf("minishell: exit: %s: numeric argument required\n", argv[1]);
		free_env(g_env);
		exit(2);
	}
	if (argv[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	free_env(g_env);
	exit((unsigned char)code);
}
