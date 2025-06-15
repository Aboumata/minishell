/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env_V1.c                                     :+:      :+:    :+:   */
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
