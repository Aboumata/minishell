/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:33:15 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/14 14:33:16 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_cd(char *path)
{
	if (!path)
	{
		printf("cd: missing operand\n");
		return (1);
	}
	if (chdir(path) == -1)
	{
		printf("cd: %s: No such file or directory\n", path);
		return (1);
	}
	return (0);
}
