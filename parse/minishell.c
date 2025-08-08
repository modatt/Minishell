/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/08/08 16:04:27 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
int	handle_command(t_shell *shell, char *line)
{
	t_command	*cmd;

	if (!line || line[0] == 0)
		return (0);
	cmd = parser(line, shell);
	if (!cmd)
		return (0);
	 print_command_list(cmd);
	if ((!cmd->arg || cmd->arg[0] == 0) && (cmd->redir_count < 0))
		return (0);
	if ((!cmd->arg || cmd->arg[0] == 0) && (cmd->redir_count > 0))
	{
		maybe_preprocess_heredocs(cmd);
		run_with_redirection(cmd, shell);
		return (1);
	}
	if (cmd->next || cmd->is_pipe)
	{
		maybe_preprocess_heredocs(cmd);
		if (g_signal_status != 130)
			execute_pipeline(cmd, shell);
	}
	else
		execute_cmd(cmd, shell);
	free_cmd(cmd);
	return (1);
}

// function - 3
int	interactive(t_shell *shell, char **command_line)
{
	(void)shell;
	*command_line = readline("minishell$ ");
	if (*command_line == NULL)
		return (0);
	if (**command_line)
		add_history(*command_line);
	else
		return (2);
	return (1);
}

// function - 4
int	non_interactive(t_shell *shell, char **command_line)
{
	*command_line = readline_non_interactive(STDIN_FILENO);
	if (!*command_line)
	{
		shell->last_exit_status = 0;
		return (0);
	}
	if (!**command_line)
	{
		free(*command_line);
		return (2);
	}
	return (1);
}

// function - 6
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		stat;

	shell = create_shell(argc, argv, envp);
	if (!shell)
		return (1);
	if (shell->is_interactive)
		greets_minishell();
	else if (setup_non_interactive_signals() == -1)
	{
		free(shell);
		exit(EXIT_FAILURE);
	}
	main_loop(shell);
	stat = shell->last_exit_status;
	free_shell(shell);
	clear_history();
	return (stat);
}
