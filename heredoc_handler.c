/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/28 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection_structures.h"

char	*generate_temp_filename(void)
{
	static int	counter = 0;
	char		*filename;
	char		*counter_str;
	size_t		total_len;

	counter_str = ft_itoa(counter);
	if (!counter_str)
		return (NULL);
	total_len = ft_strlen("/tmp/heredoc_") + ft_strlen(counter_str) + 1;
	filename = malloc(total_len);
	if (!filename)
	{
		free(counter_str);
		return (NULL);
	}
	ft_strlcpy(filename, "/tmp/heredoc_", total_len);
	ft_strlcat(filename, counter_str, total_len);
	free(counter_str);
	counter++;
	return (filename);
}

int	write_heredoc_content(int fd, const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (safe_strcmp(line, delimiter))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

int	create_heredoc_file(const char *delimiter)
{
	char	*temp_filename;
	int		fd;
	int		result;

	temp_filename = generate_temp_filename();
	if (!temp_filename)
		return (-1);
	fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(temp_filename);
		return (-1);
	}
	result = write_heredoc_content(fd, delimiter);
	close(fd);
	if (result == -1)
	{
		unlink(temp_filename);
		free(temp_filename);
		return (-1);
	}
	fd = open(temp_filename, O_RDONLY);
	unlink(temp_filename);
	free(temp_filename);
	return (fd);
}

void	cleanup_heredoc_file(const char *filename)
{
	if (filename)
		unlink(filename);
}

int	handle_heredoc(const char *delimiter)
{
	int	fd;

	if (!delimiter)
		return (-1);
	fd = create_heredoc_file(delimiter);
	if (fd == -1)
	{
		perror("heredoc creation failed");
		return (-1);
	}
	return (fd);
}
