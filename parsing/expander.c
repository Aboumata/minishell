/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 19:09:07 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/15 19:09:08 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*get_var_value(const char *var, t_envs *env, int last_status)
{
	t_envs	*node;
	char	*result;

	if (!var)
		return (ft_strdup(""));
	if (safe_strcmp(var, "?"))
		return (ft_itoa(last_status));
	node = get_env(env, var);
	if (node && node->values)
	{
		result = ft_strdup(node->values);
		return (result);
	}
	result = ft_strdup("");
	return (result);
}

int	is_var_char(char c, int first)
{
	if (first)
		return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
	return (is_var_char(c, 1) || (c >= '0' && c <= '9'));
}

int	write_value(const char *val, char *out, int *j)
{
	int	k;

	k = 0;
	while (val[k] && *j < 4095)
	{
		out[*j] = val[k];
		(*j)++;
		k++;
	}
	return (k);
}

void	process_dollar(t_exp_ctx *ctx)
{
	int	skip;

	(*ctx->i)++;
	ctx->idx = *(ctx->i);
	skip = replace_variable(ctx);
	if (skip)
		*(ctx->i) += skip;
	else
		ctx->out[(*ctx->j)++] = '$';
}
