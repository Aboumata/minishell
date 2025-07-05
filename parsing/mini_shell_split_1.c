/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_split_1.c                               :+:      :+:    :+:   */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:40:39 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/14 16:40:41 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	get_token_len(const char *s, int start)
{
	int		i;
	char	quote;

	i = start;
	quote = 0;
	while (s[i] && (quote || (!is_whitespace(s[i]) && !is_special(s, i))))
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		i++;
	}
	return (i - start);
}

static char	*extract_token(const char *s, int *i)
{
	int		special_len;
	char	*res;
	int		start;
	int		len;

	skip_spaces(s, i);
	if (!s[*i])
		return (NULL);
	if ((special_len = is_special(s, *i)))
	{
		res = malloc(special_len + 1);
		if (!res)
			return (NULL);
		ft_memcpy(res, &s[*i], special_len);
		res[special_len] = 0;
		*i += special_len;
		return (res);
	}
	start = *i;
	len = get_token_len(s, start);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, &s[start], len);
	res[len] = 0;
	*i += len;
	return (res);
}

static int	count_args(const char *s)
{
	int		i;
	char	*tok;
	int		count;

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
	int		num;
	int		i;
	int		k;

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
