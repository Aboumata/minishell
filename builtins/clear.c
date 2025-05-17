/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:16:58 by aboumata          #+#    #+#             */
/*   Updated: 2025/05/17 12:16:59 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	putchar_tc(int c)
{
	return ((int)write(1, &c, 1));
}

int	builtin_clear(void)
{
	char	*term_type;
	char	term_buffer[2048];
	char	*clear_string;

	term_type = getenv("TERM");
	if (!term_type)
	{
		printf("minishell: clear: TERM environment variable not set\n");
		return (1);
	}
	if (tgetent(term_buffer, term_type) != 1)
	{
		printf("minishell: clear: Could not access the termcap database\n");
		return (1);
	}
	clear_string = tgetstr("cl", NULL);
	if (!clear_string)
	{
		printf("minishell: clear: Terminal does not support clear screen\n");
		return (1);
	}
	tputs(clear_string, 1, putchar_tc);
	return (0);
}
