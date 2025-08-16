/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:48:05 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/05 13:48:19 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parsing.h"

static char	*expand_and_strip(const char *value_raw)
{
	char	*expanded;
	char	*result;

	expanded = expand_variables(value_raw, g_env, g_last_status);
	if (!expanded)
		return (ft_strdup(""));
	result = strip_quotes(expanded);
	free(expanded);
	if (!result)
		return (ft_strdup(""));
	return (result);
}

static void	handle_env_assignment(t_envs **env, char *name, char *value,
		int is_append)
{
	t_envs	*node;
	char	*old;
	char	*new_val;

	node = get_env(*env, name);
	if (node)
	{
		if (is_append)
		{
			if (node->values)
				old = node->values;
			else
				old = "";
			new_val = ft_strjoin(old, value);
			free(node->values);
			node->values = new_val;
		}
		else
		{
			free(node->values);
			node->values = ft_strdup(value);
		}
	}
	else
		*env = add_env(*env, name, value);
}

void	set_env_var(t_envs **env, const char *arg)
{
	t_envs	*node;
	char	*equal;
	char	*plus_equal;
	char	*name;
	char	*value;

	plus_equal = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (plus_equal)
	{
		name = ft_substr(arg, 0, plus_equal - arg);
		value = expand_and_strip(plus_equal + 2);
		handle_env_assignment(env, name, value, 1);
		return (free(name), free(value));
	}
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		name = ft_substr(arg, 0, equal - arg);
		value = expand_and_strip(equal + 1);
		handle_env_assignment(env, name, value, 0);
		return (free(name), free(value));
	}
	node = get_env(*env, arg);
	if (!node)
		*env = add_env(*env, arg, NULL);
}

static int	process_arg_validation(t_envs **env, char *trimmed_arg)
{
	if (!is_valid(trimmed_arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(trimmed_arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	set_env_var(env, trimmed_arg);
	return (0);
}

int	builtin_export(t_envs **env, char *arg[])
{
	int		i;
	char	*trimmed_arg;
	int		status;

	status = 0;
	if (!arg[1])
		return (sorted_env(*env), 0);
	i = 1;
	while (arg[i])
	{
		trimmed_arg = ft_strtrim(arg[i], " \t\n\r");
		if (trimmed_arg && trimmed_arg[0] != '\0')
		{
			if (process_arg_validation(env, trimmed_arg))
				status = 1;
		}
		free(trimmed_arg);
		i++;
	}
	return (status);
}
