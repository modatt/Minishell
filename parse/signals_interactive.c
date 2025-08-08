/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:59:08 by modat             #+#    #+#             */
/*   Updated: 2025/08/08 13:37:44 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

// function - 1
void	signals_prompt(void)
{
	setup_sig(SIGINT, handler_prompt);
	setup_sig(SIGQUIT, SIG_IGN);
}

// function - 2
void	handler_parent(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

// function - 3
void	handler_prompt(int sig)
{
	(void)sig;
	write(1, "me", 2);
	g_signal_status = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// function - 4
void	signals_execution(void)
{
	setup_sig_exc(SIGINT, SIG_DFL);
	setup_sig_exc(SIGQUIT, SIG_DFL);
}

// function - 5
void	handler_parent_quit(int sig)
{
	(void)sig;
	write(1, "Quit (core dumped)\n", 20);
}
