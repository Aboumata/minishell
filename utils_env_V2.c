/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env_V2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 14:53:37 by aboumata          #+#    #+#             */
/*   Updated: 2025/06/15 14:53:52 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envs	*copy_env_node(t_envs *node)
{
	t_envs	*new_node;

	if (!node)
		return (NULL);
	new_node = malloc(sizeof(t_envs));
	if (!new_node)
		return (NULL);
	new_node->names = ft_strdup(node->names);
	if (!new_node->names)
		return (free(new_node), NULL);
	if (node->values)
	{
		new_node->values = ft_strdup(node->values);
		if (!new_node->values)
		{
			free(new_node->names);
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->values = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_envs	*copy_env_list(t_envs *env)
{
	t_envs	*new_head;
	t_envs	*new_tail;
	t_envs	*new_node;

	new_head = NULL;
	new_tail = NULL;
	while (env)
	{
		new_node = copy_env_node(env);
		if (!new_node)
			return (free_env(new_head), NULL);
		if (!new_head)
		{
			new_head = new_node;
			new_tail = new_node;
		}
		else
		{
			new_tail->next = new_node;
			new_tail = new_node;
		}
		env = env->next;
	}
	return (new_head);
}
