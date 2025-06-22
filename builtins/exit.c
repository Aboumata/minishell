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

static int	is_valid_number(const char *str)
{
	int	i;
	int	sign;

	if (!str || *str == '\0')
		return (0);
	i = 0;
	sign = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		sign = 1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			while (str[i] == ' ' || str[i] == '\t')
				i++;
			return (str[i] == '\0');
		}
		i++;
	}
	return (1);
}

static long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	builtin_exit(char **argv)
{
	long long	code;

	printf("exit\n");
	if (!argv[1])
	{
		free_env(g_env);
		exit(0);
	}
	if (!is_valid_number(argv[1]))
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
	code = ft_atoll(argv[1]);
	if (code < 0 || code > 255)
	{
		printf("minishell: exit: %s: numeric argument required\n", argv[1]);
		free_env(g_env);
		exit(2);
	}
	free_env(g_env);
	exit((unsigned char)code);
}
