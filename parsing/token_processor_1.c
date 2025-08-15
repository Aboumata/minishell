/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/08/08 16:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	count_tokens(char **tokens)
{
	int	count;

	if (!tokens)
		return (0);
	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

char	**allocate_token_array(int count)
{
	char	**expanded_tokens;

	expanded_tokens = malloc(sizeof(char *) * (count + 1));
	if (!expanded_tokens)
		return (NULL);
	return (expanded_tokens);
}

void	cleanup_expanded_tokens(char **expanded_tokens, int index)
{
	while (index > 0)
		free(expanded_tokens[--index]);
	free(expanded_tokens);
}

char	*process_regular_token(char *token, t_envs *env, int last_status)
{
	char	*expanded;
	char	*result;

	expanded = expand_variables(token, env, last_status);
	if (expanded)
	{
		result = strip_quotes(expanded);
		free(expanded);
		return (result);
	}
	return (ft_strdup(""));
}

static int	is_heredoc_delimiter_position(char **tokens, int i)
{
	if (i == 0)
		return (0);
	if (tokens[i - 1] && ft_strncmp(tokens[i - 1], "<<", 2) == 0
		&& ft_strlen(tokens[i - 1]) == 2)
		return (1);
	return (0);
}

static int	token_has_quotes(const char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' || token[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

static char	*process_heredoc_delimiter(char *token)
{
	char	*result;
	char	*unquoted;
	size_t	len;

	if (token_has_quotes(token))
	{
		unquoted = strip_quotes(token);
		if (!unquoted)
			return (ft_strdup(""));
		len = ft_strlen(unquoted);
		result = malloc(len + 2);
		if (!result)
		{
			free(unquoted);
			return (ft_strdup(""));
		}
		result[0] = '\x01';
		ft_strlcpy(result + 1, unquoted, len + 1);
		free(unquoted);
		return (result);
	}
	else
		return (ft_strdup(token));
}

char	*process_single_token(char *token, t_envs *env, int last_status)
{
	if (is_special(token, 0))
		return (ft_strdup(token));
	return (process_regular_token(token, env, last_status));
}

char	*process_single_token_with_context(char *token, t_envs *env,
		int last_status, char **all_tokens, int current_index)
{
	if (is_special(token, 0))
		return (ft_strdup(token));
	if (is_heredoc_delimiter_position(all_tokens, current_index))
		return (process_heredoc_delimiter(token));
	return (process_regular_token(token, env, last_status));
}
