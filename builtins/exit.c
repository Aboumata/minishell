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
#include <limits.h>
#include <errno.h>

static int is_valid_number(const char *str)
{
    int i;

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

static int ft_atoll_safe(const char *str, long long *result)
{
    long long num;
    int sign;
    int i;

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
        int digit = str[i] - '0';
        if (sign == 1)
        {
            if (num > (LLONG_MAX - digit) / 10)
                return (0);
        }
        else
        {
            if (num > (LLONG_MAX - digit) / 10)
                return (0);
        }
        num = num * 10 + digit;
        i++;
    }

    *result = num * sign;
    return (1);
}

int builtin_exit(char **argv)
{
    long long code;

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
    if (!ft_atoll_safe(argv[1], &code))
    {
        printf("minishell: exit: %s: numeric argument required\n", argv[1]);
        free_env(g_env);
        exit(2);
    }
    free_env(g_env);
    exit((unsigned char)code);
}
