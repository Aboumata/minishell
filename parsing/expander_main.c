/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:09:07 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/15 19:09:08 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	handle_single_quotes(const char *input, int *i, char *out, int *j)
{
	out[(*j)++] = input[(*i)++];
	while (input[*i] && input[*i] != '\'' && *j < 4095)
		out[(*j)++] = input[(*i)++];
	if (input[*i] == '\'')
		out[(*j)++] = input[(*i)++];
}

static void	handle_double_quotes(const char *input, int *i, char *out, int *j,
		t_envs *env, int last_status)
{
	t_exp_ctx	ctx;

	out[(*j)++] = input[(*i)++];
	ctx.input = input;
	ctx.env = env;
	ctx.last_status = last_status;
	ctx.out = out;
	ctx.j = j;
	ctx.i = i;
	while (input[*i] && input[*i] != '"' && *j < 4095)
	{
		if (input[*i] == '$')
			process_dollar(&ctx);
		else
			out[(*j)++] = input[(*i)++];
	}
	if (input[*i] == '"')
		out[(*j)++] = input[(*i)++];
}

static void	handle_unquoted(const char *input, int *i, char *out, int *j,
		t_envs *env, int last_status)
{
	t_exp_ctx	ctx;

	ctx.input = input;
	ctx.env = env;
	ctx.last_status = last_status;
	ctx.out = out;
	ctx.j = j;
	ctx.i = i;
	if (input[*i] == '$')
		process_dollar(&ctx);
	else
		out[(*j)++] = input[(*i)++];
}

char	*expand_variables(const char *input, t_envs *env, int last_status)
{
	char	*out;
	int		i;
	int		j;

	out = malloc(4096);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i] && j < 4095)
	{
		if (input[i] == '\'')
			handle_single_quotes(input, &i, out, &j);
		else if (input[i] == '"')
			handle_double_quotes(input, &i, out, &j, env, last_status);
		else
			handle_unquoted(input, &i, out, &j, env, last_status);
	}
	out[j] = '\0';
	return (out);
}
