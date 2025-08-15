/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:08:49 by aboumata          #+#    #+#             */
/*   Updated: 2025/08/02 15:08:50 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal_flag = 2;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

int	is_heredoc_interrupted(void)
{
	return (g_signal_flag == 2);
}
