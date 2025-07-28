/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:53:47 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/07/28 11:58:18 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_builtin_with_redirection(t_command *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;

	if (!isatty(0))
		dup2(2, 0);
	signals_prompt();
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stderr = dup(STDERR_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0 || saved_stderr < 0)
	{
		printf("are we here??\n");
		perror("dup");
		return ;
	}
	setup_redirection_fds(cmd);
	exec_builtin(cmd, shell);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
}

void	execute_cmd(t_command *cmd, t_shell *shell)
{
	if (!cmd)
		return ;
	maybe_preprocess_heredocs(cmd);
	if (!cmd->arg || !cmd->arg[0])
	{
		return ;
	}
	if (is_builtin(cmd->arg[0]))
	{
		if (cmd->redir_count > 0 && (g_heredoc_interrupted != 130))
			run_builtin_with_redirection(cmd, shell);
		else
		{
			if (g_heredoc_interrupted != 130)
				exec_builtin(cmd, shell);
		}
	}
	else
		exec_external(cmd, shell);
}
