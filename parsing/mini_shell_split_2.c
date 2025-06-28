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
