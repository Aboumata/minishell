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

bool	is_valid(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0' || str[0] == '=' || str[0] == ' '
		|| (str[0] >= '0' && str[0] <= '9'))
		return (false);
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (i == 0)
		{
			if (!(ft_isalpha(str[i]) || str[i] == '_'))
				return (false);
		}
		else
		{
			if (!(ft_isalnum(str[i]) || str[i] == '_'))
				return (false);
		}
		if (str[i] == ' ')
			return (false);
		i++;
	}
	if (str[i] == '+' && str[i + 1] != '=')
		return (false);
	if (i == 0)
		return (false);
	return (true);
}

void	set_env_var(t_envs **env, const char *arg)
{
	char *name;
	char *equal;
	char *value;
	char *plus_equal;
	t_envs *node;

	plus_equal = ft_strnstr(arg, "+=", 0);
	if (plus_equal)
	{
		name = ft_substr(arg, 0, plus_equal - arg);
		value = plus_equal + 2;
		node = get_env(*env, name);
		if (node)
		{
			free(node->values);
			node->values = ft_strdup(value);
		}
		else
			*env = add_env(*env, name, value);
		free(name);
		return;
	}
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		name = ft_substr(arg, 0, equal - arg);
		value = equal + 1;
		node = get_env(*env, name);
		if (node)
		{
			free(node->values);
			node->values = ft_strdup(value);
		}
		else
			*env = add_env(*env, name, value);
		free(name);
		return;
	}

	if (!get_env(*env, arg))
		*env = add_env(*env, arg, "");
}

void	into_export(t_envs **env, char *arg[])
{
	int	i;

	if (!arg[1])
	{
		sorted_env(*env);
		return ;
	}
	i = 1;
	while (arg[i])
	{
		if (!is_valid(arg[i]))
			printf("export: `%s': not a valid identifier\n", arg[i]);
		else
			set_env_var(env, arg[i]);
		i++;
	}
}
