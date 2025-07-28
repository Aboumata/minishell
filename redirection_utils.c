/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/28 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection_structures.h"

void	free_redirection(t_redirection *redir)
{
	t_redirection	*current;
	t_redirection	*next;

	current = redir;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		if (current->delimiter)
			free(current->delimiter);
		free(current);
		current = next;
	}
}

void	free_cmd_with_redir(t_cmd_with_redir *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		free_split(cmd->args);
	if (cmd->redirections)
		free_redirection(cmd->redirections);
	free(cmd);
}

int	count_non_redir_tokens(char **tokens)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (tokens[i])
	{
		if (is_redirection_token(tokens[i]))
			i += 2;
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

char	**extract_clean_args(char **tokens)
{
	char	**clean_args;
	int		count;
	int		i;
	int		j;

	count = count_non_redir_tokens(tokens);
	if (count == 0)
		return (NULL);
	clean_args = malloc(sizeof(char *) * (count + 1));
	if (!clean_args)
		return (NULL);
	i = 0;
	j = 0;
	while (tokens[i] && j < count)
	{
		if (is_redirection_token(tokens[i]))
			i += 2;
		else
		{
			clean_args[j] = ft_strdup(tokens[i]);
			if (!clean_args[j])
				return (free_split(clean_args), NULL);
			i++;
			j++;
		}
	}
	clean_args[j] = NULL;
	return (clean_args);
}

t_redirection	*add_redirection_to_list(t_redirection *head,
		t_redirection *new_redir)
{
	t_redirection	*current;

	if (!new_redir)
		return (head);
	if (!head)
		return (new_redir);
	current = head;
	while (current->next)
		current = current->next;
	current->next = new_redir;
	return (head);
}

void	restore_stdio(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
