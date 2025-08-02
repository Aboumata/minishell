/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:08:49 by aboumata          #+#    #+#             */
/*   Updated: 2025/08/02 15:08:50 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

int	g_signal_flag = 0;

void	handle_sigint_interactive(int sig)
{
    (void)sig;
    g_signal_flag = 1;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void	handle_sigquit_interactive(int sig)
{
    (void)sig;
}

void	setup_interactive_signals(void)
{
    signal(SIGINT, handle_sigint_interactive);
    signal(SIGQUIT, handle_sigquit_interactive);
}

void	reset_signal_flag(void)
{
    g_signal_flag = 0;
}

int	check_signal_flag(void)
{
    return (g_signal_flag);
}
