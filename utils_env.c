/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:11:13 by aboumata          #+#    #+#             */
/*   Updated: 2025/04/30 09:11:16 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lstsize_env(t_envs *env)
{
	int	i;

	if (!env)
		return (0);
	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

static void	fill_env_array(t_envs **array, t_envs *env, int len)
{
	int		i;
	t_envs	*cur_envs;

	i = 0;
	cur_envs = env;
	while (i < len)
	{
		array[i] = cur_envs;
		cur_envs = cur_envs->next;
		i++;
	}
}

static void	relink_env_array(t_envs **array, int len)
{
	int	i;

	i = 0;
	while (i < len - 1)
	{
		array[i]->next = array[i + 1];
		i++;
	}
	array[len - 1]->next = NULL;
}

static t_envs	*copy_env_node(t_envs *node)
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

static t_envs	*copy_env_list(t_envs *env)
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

t_envs	*sort_env(t_envs *env)
{
	int		len;
	t_envs	**array;
	t_envs	*sorted_head;

	len = ft_lstsize_env(env);
	array = malloc(sizeof(t_envs *) * len);
	if (!array)
		return (NULL);
	fill_env_array(array, env, len);
	quick_sort(array, 0, len - 1);
	relink_env_array(array, len);
	sorted_head = array[0];
	free(array);
	return (sorted_head);
}

void	sorted_env(t_envs *env)
{
	t_envs	*env_copy;
	t_envs	*sorted;
	t_envs	*tmp;

	env_copy = copy_env_list(env);
	if (!env_copy)
		return ;
	sorted = sort_env(env_copy);
	tmp = sorted;
	while (tmp)
	{
		if (tmp->values == NULL)
			printf("declare -x %s\n", tmp->names);
		else
			printf("declare -x %s=\"%s\"\n", tmp->names, tmp->values);
		tmp = tmp->next;
	}
	free_env(sorted);
}
