#include "minishell.h"
#include <stdlib.h>

static void	skip_spaces(const char *s, int *i)
{
	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
}

static int	count_quoted_token(const char *s, int *i)
{
	int		count;
	char	quote;

	count = 0;
	quote = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i] == quote)
		(*i)++;
	count = 1;
	return (count);
}

static int	count_args(const char *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		skip_spaces(s, &i);
		if (!s[i])
			break ;
		if (s[i] == '\'' || s[i] == '\"')
			count += count_quoted_token(s, &i);
		else
		{
			while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\''
				&& s[i] != '\"')
				i++;
			count++;
		}
		skip_spaces(s, &i);
	}
	return (count);
}

static int	extract_token(const char *s, int *i, char **res)
{
	int		start;
	int		end;
	int		j;
	char	quote;
	char	*str;

	j = 0;
	quote = 0;
	skip_spaces(s, i);
	if (s[*i] == '\'' || s[*i] == '\"')
	{
		quote = s[*i];
		start = *i + 1;
		end = start;
		while (s[end] && s[end] != quote)
			end++;
		str = (char *)malloc((end - start) + 1);
		if (!str)
			return (0);
		while (*i + 1 < end + 1)
			str[j++] = s[++(*i)];
		str[j] = '\0';
		(*i)++;
	}
	else
	{
		start = *i;
		end = start;
		while (s[end] && s[end] != ' ' && s[end] != '\t' && s[end] != '\''
			&& s[end] != '\"')
			end++;
		str = (char *)malloc((end - start) + 1);
		if (!str)
			return (0);
		while (*i < end)
			str[j++] = s[(*i)++];
		str[j] = '\0';
	}
	*res = str;
	return (1);
}

char	**mini_shell_split(const char *s)
{
	int		num;
	char	**out;
	int		i;
	int		k;
	int		ok;

	num = count_args(s);
	out = (char **)malloc((num + 1) * sizeof(char *));
	i = 0;
	k = 0;
	ok = 1;
	if (!out)
		return (NULL);
	while (k < num && ok)
	{
		ok = extract_token(s, &i, &out[k]);
		if (!ok)
		{
			while (k > 0)
				free(out[--k]);
			free(out);
			return (NULL);
		}
		k++;
	}
	out[k] = NULL;
	return (out);
}
