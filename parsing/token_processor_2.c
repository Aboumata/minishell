/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/26 16:00:00 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	**expand_tokens(char **tokens, t_envs *env, int last_status)
{
	char	**expanded_tokens;
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
		expanded_tokens[i] = process_single_token(tokens[i], env, last_status);
		if (!expanded_tokens[i])
		{
			cleanup_expanded_tokens(expanded_tokens, i);
			return (NULL);
		}
		i++;
	}
	expanded_tokens[count] = NULL;
	return (expanded_tokens);
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
