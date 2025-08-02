/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:08:49 by aboumata          #+#    #+#             */
/*   Updated: 2025/08/02 15:08:50 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void	setup_parent_execution_signals(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void	restore_interactive_signals(void)
{
    signal(SIGINT, handle_sigint_interactive);
    signal(SIGQUIT, handle_sigquit_interactive);
}

int	handle_child_exit_status(int status)
{
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
    {
        if (WTERMSIG(status) == SIGINT)
        {
            write(STDOUT_FILENO, "\n", 1);
            return (130);
        }
        else if (WTERMSIG(status) == SIGQUIT)
        {
            write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
            return (131);
        }
        return (128 + WTERMSIG(status));
    }
    return (-1);
}
