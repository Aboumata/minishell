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

#include "../minishell.h"

void	remove_env(t_envs **env, char *name)
{
	t_envs	*cur;
	t_envs	*prev;

	if (!env || !*env || !name)
		return ;
	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (safe_strcmp(cur->names, name))
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->names);
			free(cur->values);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

t_envs	*get_env(t_envs *env, const char *name)
{
	while (env)
	{
		if (safe_strcmp(env->names, name))
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	update_env(t_envs *env, const char *name, const char *new_value)
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

t_envs	*create_env(const char *env_strs)
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
	{
		free(new_env->names);
		free(new_env->values);
		free(new_env);
		return (NULL);
	}
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
