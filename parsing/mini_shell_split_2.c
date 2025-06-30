/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_split_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:19:06 by aboumata          #+#    #+#             */
/*   Updated: 2025/06/24 10:19:07 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_special(const char *s, int i)
{
	if (s[i] == '|')
		return (1);
	if (s[i] == '<' && s[i + 1] == '<')
		return (2);
	if (s[i] == '>' && s[i + 1] == '>')
		return (2);
	if (s[i] == '<' || s[i] == '>')
		return (1);
	return (0);
}

void	skip_spaces(const char *s, int *i)
{
	while (s[*i] && is_whitespace(s[*i]))
		(*i)++;
}

char	*extract_special_token(const char *s, int *i)
{
	int		special_len;
	char	*res;

	special_len = is_special(s, *i);
	if (!special_len)
		return (NULL);
	res = malloc(special_len + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, &s[*i], special_len);
	res[special_len] = 0;
	*i += special_len;
	return (res);
}

char	*extract_quoted_token(const char *s, int *i)
{
	char	quote;
	char	*res;

	int (start), (len);
	if (s[*i] != '\'' && s[*i] != '"')
		return (NULL);
	quote = s[(*i)++];
	start = *i;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	len = *i - start;
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, &s[start], len);
	res[len] = 0;
	if (s[*i] == quote)
		(*i)++;
	return (res);
}
