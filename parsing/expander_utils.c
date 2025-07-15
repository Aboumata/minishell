/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:03:03 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/15 19:03:04 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	expand_question(t_exp_ctx *ctx)
{
	char	*val;

	val = get_var_value("?", ctx->env, ctx->last_status);
	free(val);
	return (1);
}

int	expand_name(t_exp_ctx *ctx)
{
	char	var[128];
	int		v;
	int		i;
	char	*val;

	v = 0;
	i = ctx->idx;
	while (is_var_char(ctx->input[i], v == 0) && v < 127)
	{
		var[v] = ctx->input[i];
		v++;
		i++;
	}
	var[v] = 0;
	val = get_var_value(var, ctx->env, ctx->last_status);
	write_value(val, ctx->out, ctx->j);
	free(val);
	return (i - ctx->idx);
}

int	replace_variable(t_exp_ctx *ctx)
{
	int		start;
	int		end;
	char	var[128];
	int		len;
	char	*val;

	if (!ctx->input[ctx->idx])
		return (0);
	if (ctx->input[ctx->idx] == '{')
	{
		start = ctx->idx + 1;
		end = start;
		while (ctx->input[end] && ctx->input[end] != '}' && (end - start) < 127)
			end++;
		if (ctx->input[end] != '}')
			return (0);
		len = end - start;
		if (len <= 0 || len >= 127)
			return (0);
		ft_memcpy(var, &(ctx->input[start]), len);
		var[len] = '\0';
		val = get_var_value(var, ctx->env, ctx->last_status);
		write_value(val, ctx->out, ctx->j);
		free(val);
		return (end - ctx->idx + 1);
	}
	if (ctx->input[ctx->idx] == '?')
		return (expand_question(ctx));
	if (!is_var_char(ctx->input[ctx->idx], 1))
		return (0);
	return (expand_name(ctx));
}
