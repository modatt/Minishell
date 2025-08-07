/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:53:47 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/08/07 11:37:44 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_signal_exit_status(t_shell *shell)
{
	if (g_signal_status == SIGINT)
		shell->last_exit_status = 130;
	else if (g_signal_status == SIGQUIT)
		shell->last_exit_status = 131;
	g_signal_status = 0;
}

void	run_builtin_with_redirection(t_command *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;

	if (!shell->is_interactive)
		dup2(2, 0);
	signals_prompt();
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stderr = dup(STDERR_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0 || saved_stderr < 0)
	{
		// printf("are we here??\n");
		perror("dup");
		return ;
	}
	setup_redirection_fds(cmd);
	exec_builtin(cmd, shell);
	handle_signal_exit_status(shell);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
}

void	execute_cmd(t_command *cmd, t_shell *shell)
{
	if ((!cmd || !cmd->arg || cmd->arg[0] == 0) && (cmd->redir_count < 0))
		return ;
		
	maybe_preprocess_heredocs(cmd);
	if(g_signal_status != 130)
	{
		if (cmd->arg && is_builtin(cmd->arg[0]))
		{
			if (cmd->redir_count > 0)
				run_builtin_with_redirection(cmd, shell);
			else
				exec_builtin(cmd, shell);
		}
		else
			exec_external(cmd, shell);
	}
	(void)shell;
}
