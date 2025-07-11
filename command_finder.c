/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_finder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:53:28 by aboumata          #+#    #+#             */
/*   Updated: 2025/06/19 14:53:29 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_exec.h"

char	*build_full_path(char *directory, char *command)
{
	size_t	total;
	char	*full_path;
	int		i;
	int		j;

	total = ft_strlen(directory) + ft_strlen(command);
	full_path = (char *)malloc(sizeof(char) * (total + 2));
	if (!full_path)
		return (NULL);
	j = 0;
	i = 0;
	while (directory[j])
		full_path[i++] = directory[j++];
	full_path[i++] = '/';
	j = 0;
	while (command[j])
		full_path[i++] = command[j++];
	full_path[i] = '\0';
	return (full_path);
}

int	is_executable(const char *path)
{
	if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
		return (1);
	return (0);
}

char	*find_executable(char *command)
{
	char	**directories;
	char	*full_path;
	int		i;

	if (command[0] == '/' || command[0] == '.')
	{
		if (is_executable(command))
			return (ft_strdup(command));
		return (NULL);
	}
	directories = get_path_directories();
	if (!directories)
		return (NULL);
	i = 0;
	while (directories[i])
	{
		full_path = build_full_path(directories[i], command);
		if (full_path && is_executable(full_path))
		{
			free_path_directories(directories);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_path_directories(directories);
	return (NULL);
}
