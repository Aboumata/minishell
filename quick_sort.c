/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:43:03 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/05 18:43:07 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	envs_name_cmp(const char *a, const char *b)
{
	size_t	len_a;
	size_t	len_b;
	size_t	cmp_len;
	int		result;

	len_a = ft_strlen(a);
	len_b = ft_strlen(b);
	if (len_a > len_b)
		cmp_len = len_a;
	else
		cmp_len = len_b;
	result = ft_strncmp(a, b, cmp_len);
	if (result == 0)
	{
		if (len_a < len_b)
			return (-1);
		else if (len_a > len_b)
			return (1);
		else
			return (0);
	}
	return (result);
}

void	swap(t_envs **a, t_envs **b)
{
	t_envs	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	partition(t_envs **arr, const int low, const int high)
{
	t_envs	*pivot;
	int		i;
	int		j;

	pivot = arr[high];
	i = low - 1;
	j = low;
	while (j < high)
	{
		if (envs_name_cmp(arr[j]->names, pivot->names) < 0)
		{
			i++;
			swap(&arr[i], &arr[j]);
		}
		j++;
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void	quick_sort(t_envs **arr, const int low, const int high)
{
	int	pi;

	if (low < high)
	{
		pi = partition(arr, low, high);
		quick_sort(arr, low, pi - 1);
		quick_sort(arr, pi + 1, high);
	}
}
