/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:00:00 by aboumata          #+#    #+#             */
/*   Updated: 2025/07/26 16:00:00 by aboumata         ###   ########.fr       */
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

char	*process_single_token(char *token, t_envs *env, int last_status)
{
    if (is_special(token, 0))
        return (ft_strdup(token));
    return (process_regular_token(token, env, last_status));
}
