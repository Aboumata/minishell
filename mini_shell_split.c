/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_shell_split.c                                 :+:      :+:    :+:   */
/*   By: aboumata                                       +#+  +:+       +#+      */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_spaces(const char *s, int *i)
{
    while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
        (*i)++;
}

static int	count_args(const char *s)
{
    int	i = 0;
    int	count = 0;
    bool	in_word = false;
    char	quote = 0;

    while (s[i])
    {
        skip_spaces(s, &i);
        if (!s[i])
            break ;
        in_word = true;
        if (s[i] == '\'' || s[i] == '\"')
        {
            quote = s[i++];
            while (s[i] && s[i] != quote)
                i++;
            if (s[i] == quote)
                i++;
        }
        else
        {
            while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\'' && s[i] != '\"')
                i++;
        }
        count++;
        skip_spaces(s, &i);
    }
    return (count);
}

static char	*extract_token(const char *s, int *i)
{
    int		start, end, j = 0;
    char	quote = 0;
    char	*res;

    skip_spaces(s, i);
    if (s[*i] == '\'' || s[*i] == '\"')
    {
        quote = s[*i];
        start = ++(*i);
        end = start;
        while (s[end] && s[end] != quote)
            end++;
        res = (char *)malloc(end - start + 1);
        if (!res)
            return NULL;
        while (*i < end)
            res[j++] = s[(*i)++];
        res[j] = 0;
        if (s[*i] == quote)
            (*i)++;
    }
    else
    {
        start = *i;
        while (s[*i] && s[*i] != ' ' && s[*i] != '\t' && s[*i] != '\'' && s[*i] != '\"')
            (*i)++;
        res = (char *)malloc(*i - start + 1);
        if (!res)
            return NULL;
        while (start < *i)
            res[j++] = s[start++];
        res[j] = 0;
    }
    return res;
}

char	**mini_shell_split(const char *s)
{
    int		num = count_args(s);
    char	**out = (char **)malloc((num + 1) * sizeof(char *));
    int		i = 0;
    int		k = 0;

    if (!out)
        return NULL;
    while (k < num)
    {
        out[k] = extract_token(s, &i);
        if (!out[k])
        {
            while (k > 0)
                free(out[--k]);
            free(out);
            return NULL;
        }
        k++;
    }
    out[k] = NULL;
    return out;
}
