/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/28 14:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redirection_structures.h"

int	is_redirection_token(const char *token)
{
	if (!token)
		return (0);
	if (safe_strcmp(token, "<"))
		return (1);
	if (safe_strcmp(token, ">"))
		return (1);
	if (safe_strcmp(token, ">>"))
		return (1);
	if (safe_strcmp(token, "<<"))
		return (1);
	return (0);
}

t_redir_type	get_redirection_type(const char *token)
{
	if (safe_strcmp(token, "<"))
		return (REDIR_IN);
	if (safe_strcmp(token, ">"))
		return (REDIR_OUT);
	if (safe_strcmp(token, ">>"))
		return (REDIR_APPEND);
	if (safe_strcmp(token, "<<"))
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

t_redirection	*create_redirection(t_redir_type type, const char *target)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->next = NULL;
	redir->heredoc_fd = -1;
	if (type == REDIR_HEREDOC)
	{
		redir->delimiter = ft_strdup(target);
		redir->file = NULL;
	}
	else
	{
		redir->file = ft_strdup(target);
		redir->delimiter = NULL;
	}
	if ((type == REDIR_HEREDOC && !redir->delimiter) || (type != REDIR_HEREDOC
			&& !redir->file))
	{
		free_redirection(redir);
		return (NULL);
	}
	return (redir);
}

int	has_redirections(char **tokens)
{
	int	i;

	if (!tokens)
		return (0);
	i = 0;
	while (tokens[i])
	{
		if (is_redirection_token(tokens[i]))
			return (1);
		i++;
	}
	return (0);
}

t_redirection	*parse_redirections(char **tokens, char ***clean_args)
{
	t_redirection	*head;
	t_redirection	*new_redir;
	t_redir_type	type;
	int				i;

	head = NULL;
	i = 0;
	while (tokens[i])
	{
		if (is_redirection_token(tokens[i]))
		{
			if (!tokens[i + 1])
				return (free_redirection(head), NULL);
			type = get_redirection_type(tokens[i]);
			new_redir = create_redirection(type, tokens[i + 1]);
			if (!new_redir)
				return (free_redirection(head), NULL);
			head = add_redirection_to_list(head, new_redir);
			i += 2;
		}
		else
			i++;
	}
	*clean_args = extract_clean_args(tokens);
	return (head);
}
