/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:16:46 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/07 17:16:47 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
