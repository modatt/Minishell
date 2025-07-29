/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_handler_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:23:09 by modat             #+#    #+#             */
/*   Updated: 2025/07/29 12:23:55 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_heredoc_interrupted = 0;

// func 1
void	handler_parent(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}
// func 2
void	signals_prompt(void)
{
	setup_sig(SIGINT, handler_prompt);
	setup_sig(SIGQUIT, SIG_IGN);
}

// func 3
void	signals_execution(void)
{
	setup_sig(SIGINT, SIG_DFL);
	setup_sig_exc(SIGQUIT, SIG_DFL);
}

// func 4
void	handler_parent_quit(int sig)
{
	(void)sig;
	write(1, "Quit (core dumped)\n", 20);
}

// func 5
void	handler_heredoc(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 130;
	write(1, "^C", 3);
	close(0);
	signals_prompt();
}
