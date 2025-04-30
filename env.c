/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:31:31 by aboumata          #+#    #+#             */
/*   Updated: 2025/04/26 12:31:33 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env(t_envs **env, char *name)
{
	t_envs	*current;
	t_envs	*previous;

	if (!env || !*env || !name)
		return ;
	current = *env;
	previous = NULL;
	if (safe_strcmp(current->names, name))
	{
		*env = current->next;
		free(current->names);
		free(current->values);
		free(current);
		return ;
	}
	while (current)
	{
		if (safe_strcmp(current->names, name))
		{
			if (previous != NULL)
				previous->next = current->next;
			free(current->names);
			free(current->values);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

t_envs	*get_env(t_envs *env, char *name)
{
	while (env)
	{
		if (ft_strncmp(env->names, name, ft_strlen(name)) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	update_env(t_envs *env, char *name, char *new_value)
{
	t_envs	*node;
	char	*joined;
	char	*temp;

	node = get_env(env, name);
	if (node)
	{
		temp = ft_strjoin(node->values, ":");
		joined = ft_strjoin(temp, new_value);
		free(temp);
		free(node->values);
		node->values = joined;
	}
}

t_envs	*create_env(char *env_strs)
{
	t_envs	*new_env;
	char	*equal;

	equal = ft_strchr(env_strs, '=');
	if (!equal)
		return (NULL);
	new_env = (t_envs *)malloc(sizeof(t_envs));
	if (!new_env)
		return (NULL);
	new_env->names = ft_substr(env_strs, 0, equal - env_strs);
	new_env->values = ft_strdup(equal + 1);
	if (!new_env->names || !new_env->values)
		return (NULL);
	new_env->next = NULL;
	return (new_env);
}

t_envs	*init_env(char *envp[])
{
	int		i;
	t_envs	*env;
	t_envs	*new_env;

	env = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		new_env = create_env(envp[i]);
		if (new_env != NULL)
		{
			new_env->next = env;
			env = new_env;
		}
		i++;
	}
	return (env);
}
