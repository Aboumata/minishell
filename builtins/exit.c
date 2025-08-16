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
	if (!str[i] || !ft_isdigit(str[i]))
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

static void	cleanup_and_exit(int code)
{
	clear_history();
	free_env(g_env);
	g_env = NULL;
	exit(code);
}

static void	handle_exit_errors(char *arg, int error_type)
{
	if (error_type == 1)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup_and_exit(2);
	}
	else if (error_type == 2)
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
}

int	builtin_exit(char **argv)
{
	long long	code;

	printf("exit\n");
	if (!argv[1])
		cleanup_and_exit(0);
	if (!is_valid_number(argv[1]) || !ft_atoll_safe(argv[1], &code))
		handle_exit_errors(argv[1], 1);
	if (argv[2])
	{
		handle_exit_errors(NULL, 2);
		return (1);
	}
	cleanup_and_exit((unsigned char)code);
	return (0);
}
