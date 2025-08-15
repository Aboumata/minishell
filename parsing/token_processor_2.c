/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/08/08 16:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static char	**filter_empty_tokens(char **tokens)
{
	char	**filtered;
	int		valid_count;
	int		i;
	int		j;

	if (!tokens)
		return (NULL);
	valid_count = 0;
	i = 0;
	while (tokens[i])
	{
		if (tokens[i][0] != '\0')
			valid_count++;
		i++;
	}
	filtered = malloc(sizeof(char *) * (valid_count + 1));
	if (!filtered)
		return (NULL);
	i = 0;
	j = 0;
	while (tokens[i])
	{
		if (tokens[i][0] != '\0')
		{
			filtered[j] = ft_strdup(tokens[i]);
			if (!filtered[j])
			{
				while (j > 0)
					free(filtered[--j]);
				free(filtered);
				return (NULL);
			}
			j++;
		}
		i++;
	}
	filtered[j] = NULL;
	return (filtered);
}

char	**expand_tokens(char **tokens, t_envs *env, int last_status)
{
	char	**expanded_tokens;
	char	**filtered_tokens;
	int		i;
	int		count;

	if (!tokens)
		return (NULL);
	count = count_tokens(tokens);
	expanded_tokens = allocate_token_array(count);
	if (!expanded_tokens)
		return (NULL);
	i = 0;
	while (i < count)
	{
		expanded_tokens[i] = process_single_token_with_context(tokens[i], env,
				last_status, tokens, i);
		if (!expanded_tokens[i])
		{
			cleanup_expanded_tokens(expanded_tokens, i);
			return (NULL);
		}
		i++;
	}
	expanded_tokens[count] = NULL;
	filtered_tokens = filter_empty_tokens(expanded_tokens);
	free_tokens(expanded_tokens);
	if (!filtered_tokens)
		return (NULL);
	if (!filtered_tokens[0])
	{
		free(filtered_tokens);
		return (NULL);
	}
	return (filtered_tokens);
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

char	**parse_command_line(const char *input, t_envs *env, int last_status)
{
	char	**tokens;
	char	**expanded_tokens;

	tokens = mini_shell_split(input);
	if (!tokens)
		return (NULL);
	expanded_tokens = expand_tokens(tokens, env, last_status);
	free_tokens(tokens);
	return (expanded_tokens);
}

int	validate_input_parameters(const char *input, t_envs *env)
{
	if (!input)
		return (0);
	if (!env)
		return (0);
	return (1);
}

char	**handle_tokenization_error(void)
{
	return (NULL);
}
