/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:48:05 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/05 13:48:19 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid(char *str)
{
	int	i;

	if (!str || str[0] == '\0' || str[0] == '=' || str[0] == ' '
		|| str[0] >= '0' || str[0] <= '9')
		return (false);
	i = 0;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (i == 0 && !ft_isalpha(str[i] || str[i] != '_'))
			return (false);
		if (i > 0 && !ft_isalnum(str[i] || str[i] != '_'))
			return (false);
		i++;
	}
	return (true);
}

void	into_export(t_envs *env, char *arg[])
{
	int	i;

	if (!arg[1])
	{
		sorted_env(env);
		return ;
	}
	i = 1;
	while (arg[i])
	{
		if (!is_valid(arg[i]))
			printf("export: `%s': not a valid identifier\n", arg[i]);
		else
			i++;
	}
}
