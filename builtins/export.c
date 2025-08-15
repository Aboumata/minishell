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

extern int	g_last_status;

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
		else if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (false);
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

static void	set_append_env_var(t_envs **env, const char *arg, char *plus_equal)
{
	t_envs	*node;
	char	*name;
	char	*value;
	char	*old;
	char	*new_val;

	name = ft_substr(arg, 0, plus_equal - arg);
	value = expand_and_strip(plus_equal + 2);
	node = get_env(*env, name);
	if (node)
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
		*env = add_env(*env, name, value);
	free(name);
	free(value);
}

static void	set_equal_env_var(t_envs **env, const char *arg, char *equal)
{
	t_envs	*node;
	char	*name;
	char	*value;

	name = ft_substr(arg, 0, equal - arg);
	value = expand_and_strip(equal + 1);
	node = get_env(*env, name);
	if (node)
	{
		free(node->values);
		node->values = ft_strdup(value);
	}
	else
		*env = add_env(*env, name, value);
	free(name);
	free(value);
}

void	set_env_var(t_envs **env, const char *arg)
{
	t_envs	*node;
	char	*equal;
	char	*plus_equal;

	plus_equal = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (plus_equal)
	{
		set_append_env_var(env, arg, plus_equal);
		return ;
	}
	equal = ft_strchr(arg, '=');
	if (equal)
	{
		set_equal_env_var(env, arg, equal);
		return ;
	}
	node = get_env(*env, arg);
	if (!node)
		*env = add_env(*env, arg, NULL);
}

int	builtin_export(t_envs **env, char *arg[])
{
	int		i;
	char	*trimmed_arg;
	int		status;

	status = 0;
	if (!arg[1])
	{
		sorted_env(*env);
		return (0);
	}
	i = 1;
	while (arg[i])
	{
		trimmed_arg = ft_strtrim(arg[i], " \t\n\r");
		if (!trimmed_arg || trimmed_arg[0] == '\0')
		{
			free(trimmed_arg);
			i++;
			continue ;
		}
		if (!is_valid(trimmed_arg))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(trimmed_arg, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
			set_env_var(env, trimmed_arg);
		free(trimmed_arg);
		i++;
	}
	return (status);
}
