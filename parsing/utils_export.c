/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:49:34 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/13 14:49:36 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	safe_strcmp(const char *s1, const char *s2)
{
	size_t	len_s1;
	size_t	len_s2;

	if (!s1 || !s2)
		return (0);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if (len_s1 != len_s2)
		return (false);
	return (ft_strncmp(s1, s2, ft_strlen(s1)) == 0);
}

void	print_env(const t_envs *env)
{
	while (env)
	{
		printf("%s=%s\n", env->names, env->values);
		env = env->next;
	}
}

t_envs	*add_env(t_envs *env, const char *names, const char *values)
{
	t_envs	*new_node;
	t_envs	*current;

	new_node = (t_envs *)malloc(sizeof(t_envs));
	if (!new_node)
		return (env);
	new_node->names = ft_strdup(names);
	if (!new_node->names)
		return (free(new_node), env);
	if (values)
	{
		new_node->values = ft_strdup(values);
		if (!new_node->values)
		{
			free(new_node->names);
			free(new_node);
			return (env);
		}
	}
	else
		new_node->values = NULL;
	new_node->next = NULL;
	if (!env)
		return (new_node);
	current = env;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (env);
}

char	*strip_quotes(const char *value)
{
	size_t	len;

	if (!value)
		return (NULL);

	len = ft_strlen(value);
	if (len >= 2)
	{
		if (value[0] == '\'' && value[len - 1] == '\'')
			return (ft_substr(value, 1, len - 2));
		if (value[0] == '"' && value[len - 1] == '"')
			return (ft_substr(value, 1, len - 2));
	}
	return (ft_strdup(value));
}
