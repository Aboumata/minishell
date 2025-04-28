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

#include "minshell.h"

void	remove_env(t_envs **env, char *name)
{
	t_envs	*current;
	t_envs	*previous;

	if (!env || !*env || !name)
		return ;
	current = *env;
	previous = NULL;
	if (ft_strncmp(current->names, name, ft_strlen(name)) == 0)
	{
		*env = current->next;
		free(current->names);
		free(current->values);
		free(current);
		return ;
	}
	while (current)
	{
		if (ft_strncmp(current->names, name, ft_strlen(name)) == 0)
		{
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

void	print_env(t_envs *env)
{
	while (env)
	{
		printf("%s=%s\n", env->names, env->values);
		env = env->next;
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

	node = get_env(env, name);
	if (node)
	{
		free(node->values);
		node->values = ft_strdup(new_value);
	}
}

t_envs	*create_env(char *env_strs)
{
	t_envs	*new_env;
	char	**split;

	split = ft_split(env_strs, '=');
	if (!split || !split[0] || !split[1])
		return (NULL);
	new_env = malloc(sizeof(t_envs));
	if (!new_env)
		return (NULL);
	new_env->names = ft_strdup(split[0]);
	if (split[1])
		new_env->values = ft_strdup(split[1]);
	else
		new_env->values = ft_strdup("");
	new_env->next = NULL;
	free_split(split);
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
