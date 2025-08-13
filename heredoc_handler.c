/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/08/08 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection_structures.h"
#include "parsing/parsing.h"
#include "signals/signals.h"

static int g_heredoc_line_counter = 2;

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

static int	was_delimiter_quoted(const char *delimiter)
{
	return (delimiter && delimiter[0] == '\x01');
}

static char	*clean_delimiter(const char *delimiter, int *should_expand)
{
	char	*result;

	if (was_delimiter_quoted(delimiter))
	{
		*should_expand = 0;
		result = ft_strdup(delimiter + 1);
	}
	else
	{
		*should_expand = 1;
		result = ft_strdup(delimiter);
	}
	return (result);
}

static int	handle_heredoc_child_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			return (130);
		else if (WTERMSIG(status) == SIGQUIT)
			return (131);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

int	process_delimiter(char **delimiter, int *should_expand)
{
	char	*processed;

	if (!delimiter || !*delimiter)
		return (-1);
	processed = clean_delimiter(*delimiter, should_expand);
	if (!processed)
		return (-1);
	free(*delimiter);
	*delimiter = processed;
	return (0);
}

int	write_heredoc_content(int fd, char *delimiter, int should_expand)
{
	char		*line;
	char		*expanded_line;
	pid_t		pid;
	int			status;
	int			current_line_num;

	current_line_num = g_heredoc_line_counter;

	pid = fork();
	if (pid == 0)
	{
		setup_heredoc_signals();
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				ft_putstr_fd("bash: warning: here-document at line ", 2);
				ft_putnbr_fd(current_line_num, 2);
				ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
				ft_putstr_fd(delimiter, 2);
				ft_putstr_fd("')\n", 2);
				exit(0);
			}
			if (safe_strcmp(line, delimiter))
			{
				free(line);
				break ;
			}
			if (should_expand)
			{
				expanded_line = expand_variables(line, g_env, g_last_status);
				if (expanded_line)
				{
					write(fd, expanded_line, ft_strlen(expanded_line));
					free(expanded_line);
				}
				else
					write(fd, line, ft_strlen(line));
			}
			else
			{
				write(fd, line, ft_strlen(line));
			}
			write(fd, "\n", 1);
			free(line);
		}
		exit(0);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		g_heredoc_line_counter++;
		return (handle_heredoc_child_exit_status(status));
	}
	else
	{
		perror("fork failed");
		return (1);
	}
}

int	create_heredoc_file(const char *original_delimiter)
{
	char	*temp_filename;
	char	*delimiter;
	int		fd;
	int		result;
	int		should_expand;

	delimiter = ft_strdup(original_delimiter);
	if (!delimiter)
		return (-1);
	if (process_delimiter(&delimiter, &should_expand) == -1)
	{
		free(delimiter);
		return (-1);
	}
	temp_filename = generate_temp_filename();
	if (!temp_filename)
	{
		free(delimiter);
		return (-1);
	}
	fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(temp_filename);
		free(delimiter);
		return (-1);
	}
	result = write_heredoc_content(fd, delimiter, should_expand);
	close(fd);
	free(delimiter);
	if (result == 130)
	{
		unlink(temp_filename);
		free(temp_filename);
		g_last_status = 130;
		return (-2);
	}
	else if (result != 0)
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

int	handle_heredoc(const char *delimiter)
{
	int	fd;

	if (!delimiter)
		return (-1);
	fd = create_heredoc_file(delimiter);
	if (fd == -2)
	{
		return (-2);
	}
	if (fd == -1)
	{
		if (is_heredoc_interrupted())
		{
			g_last_status = 130;
			return (-2);
		}
		perror("heredoc creation failed");
		return (-1);
	}
	return (fd);
}

int	process_all_heredocs(t_redirection *redirections)
{
	t_redirection	*current;
	int				fd;
	int				heredoc_count;
	t_redirection	**heredoc_array;
	int				i;

	current = redirections;
	heredoc_count = 0;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
			heredoc_count++;
		current = current->next;
	}

	if (heredoc_count == 0)
		return (0);

	heredoc_array = malloc(sizeof(t_redirection *) * heredoc_count);
	if (!heredoc_array)
		return (-1);

	current = redirections;
	i = 0;
	while (current)
	{
		if (current->type == REDIR_HEREDOC)
		{
			heredoc_array[i] = current;
			i++;
		}
		current = current->next;
	}
	i = 0;
	while (i < heredoc_count)
	{
		fd = handle_heredoc(heredoc_array[i]->delimiter);
		if (fd == -2)
		{
			free(heredoc_array);
			return (-2);
		}
		if (fd == -1)
		{
			free(heredoc_array);
			return (-1);
		}
		heredoc_array[i]->heredoc_fd = fd;
		i++;
	}

	free(heredoc_array);
	return (0);
}
