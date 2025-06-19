/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:35:06 by aboumata          #+#    #+#             */
/*   Updated: 2025/06/19 11:35:08 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_exec.h"

void	free_path_directories(char **directories)
{
	int	i;

	i = 0;
	while (directories[i])
	{
		free(directories[i]);
		i++;
	}
	free(directories);
}

char	**get_path_directories(void)
{
	char	*path;
	char	**tokens;

	path = getenv("PATH");
	if (!path)
		return (NULL);
	tokens = ft_split(path, ':');
	if (!tokens)
		return (NULL);
	return (tokens);
}
