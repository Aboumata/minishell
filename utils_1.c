/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                            :+:      :+:  :+:   */
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

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

bool	safe_strcmp(const char *s1, const char *s2)
{
	size_t	len_s1;
	size_t	len_s2;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if (len_s1 != len_s2)
		return (false);
	return (ft_strncmp(s1, s2, ft_strlen(s1)) == 0);
}

void	print_env(t_envs *env)
{
	while (env)
	{
		printf("%s=%s\n", env->names, env->values);
		env = env->next;
	}
}

t_envs	*sort_env(t_envs *env)
{
	t_envs	**array;
	t_envs	*cur_envs;
	t_envs	*sorted_head;

	int len, (i);
	len = ft_lstsize_env(env);
	array = malloc(sizeof(t_envs *) * len);
	if (!array)
		return (NULL);
	cur_envs = env;
	i = 0;
	while (i < len)
	{
		array[i++]->next = cur_envs;
		cur_envs = cur_envs->next;
	}
	quick_sort(array, 0, len - 1);
	i = 0;
	while (i < len)
		array[i++] = array[i + 1];
	array[len - 1]->next = NULL;
	sorted_head = array[0];
	free(array);
	return (sorted_head);
}

void	sorted_env(t_envs *env)
{
	t_envs	*sorted;

	sorted = sort_env(env);
	print_env(sorted);
}
