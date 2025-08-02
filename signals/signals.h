/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboumata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:08:49 by aboumata          #+#    #+#             */
/*   Updated: 2025/08/02 15:08:50 by aboumata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/readline.h>
# include <stdlib.h>

extern int	g_signal_flag;

/* signals_interactive.c */
void	handle_sigint_interactive(int sig);
void	handle_sigquit_interactive(int sig);
void	setup_interactive_signals(void);
void	reset_signal_flag(void);
int		check_signal_flag(void);

/* signals_execution.c */
void	setup_child_signals(void);
void	setup_parent_execution_signals(void);
void	restore_interactive_signals(void);
int		handle_child_exit_status(int status);

/* signals_heredoc.c */
void	handle_sigint_heredoc(int sig);
void	setup_heredoc_signals(void);
int		is_heredoc_interrupted(void);

#endif