/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quote_handler.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:09:07 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/15 19:09:08 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	copy_literal_content(const char *input, int *i, char *out, int *j,
		char quote_char)
{
	while (input[*i] && input[*i] != quote_char && *j < 4095)
		out[(*j)++] = input[(*i)++];
}

void	expand_within_quotes(const char *input, int *i, char *out, int *j,
		t_exp_ctx *ctx)
{
	while (input[*i] && input[*i] != '"' && *j < 4095)
	{
		if (input[*i] == '$')
			process_dollar(ctx);
		else
			out[(*j)++] = input[(*i)++];
	}
}

void	skip_quote_delimiter(const char *input, int *i, char quote_char)
{
	if (input[*i] == quote_char)
		(*i)++;
}

void	setup_expansion_context(t_exp_ctx *ctx, const char *input, t_envs *env,
		int last_status)
{
	ctx->input = input;
	ctx->env = env;
	ctx->last_status = last_status;
}

void	process_expansion_char(const char *input, int *i, char *out, int *j,
		t_exp_ctx *ctx)
{
	if (input[*i] == '$')
		process_dollar(ctx);
	else
		out[(*j)++] = input[(*i)++];
}
