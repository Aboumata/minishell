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

	if (!str || *str == '\0')
		return (0);
	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
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

static int	ft_atoll_safe(const char *str, long long *result)
{
	long long	num;
	int			sign;
	int			i;

	num = 0;
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
		if (num > (LLONG_MAX - (str[i] - '0')) / 10)
			return (0);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	*result = num * sign;
	return (1);
}

int	builtin_exit(char **argv)
{
	long long	code;

	if (argv[1] && argv[2])
	{
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	printf("exit\n");
	clear_history();
	free_env(g_env);
	g_env = NULL;
	if (!argv[1])
		exit(0);
	if (!is_valid_number(argv[1]) || !ft_atoll_safe(argv[1], &code))
	{
		printf("minishell: exit: %s: numeric argument required\n", argv[1]);
		exit(2);
	}
	exit((unsigned char)code);
}
