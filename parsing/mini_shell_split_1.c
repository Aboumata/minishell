/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_split_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:40:39 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/14 16:40:41 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static char	*extract_unquoted_token(const char *s, int *i)
{
	char	quote;
	char	*res;

	int (start), (len);
	start = *i;
	while (s[*i] && !is_whitespace(s[*i]) && !is_special(s, *i))
	{
		if (s[*i] == '\'' || s[*i] == '"')
		{
			quote = s[(*i)++];
			while (s[*i] && s[*i] != quote)
				(*i)++;
			if (s[*i] == quote)
				(*i)++;
		}
		else
			(*i)++;
	}
	len = *i - start;
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, &s[start], len);
	res[len] = 0;
	return (res);
}

static char	*extract_token(const char *s, int *i)
{
	char	*res;

	skip_spaces(s, i);
	if (!s[*i])
		return (NULL);
	res = extract_special_token(s, i);
	if (res)
		return (res);
	res = extract_quoted_token(s, i);
	if (res)
		return (res);
	return (extract_unquoted_token(s, i));
}

static int	count_args(const char *s)
{
	int		i;
	char	*tok;
	int		count;

	i = 0;
	i = 0;
	count = 0;
	while (1)
	{
		tok = extract_token(s, &i);
		if (!tok)
			break ;
		count++;
		free(tok);
	}
	return (count);
}

char	**mini_shell_split(const char *s)
{
	char	**out;
	char	*tok;

	int (num), (i), (k);
	num = count_args(s);
	out = malloc((num + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	i = 0;
	k = 0;
	while (k < num)
	{
		tok = extract_token(s, &i);
		if (!tok)
		{
			while (k > 0)
				free(out[--k]);
			free(out);
			return (NULL);
		}
		out[k++] = tok;
	}
	out[k] = NULL;
	return (out);
}
