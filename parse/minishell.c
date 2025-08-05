/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/08/05 19:49:25 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
t_shell	*create_shell(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("Failed to allocate shell structure");
		return (NULL);
	}
	if ((init_shell(shell, argc, argv, envp) != 0))
	{
		printf("Failed to initialize shell\n");
		free(shell);
		return (NULL);
	}
	if (!shell->envp)
		printf("Warning: Failed to initialize environment\n");
	return (shell);
}
static void	run_with_redirection(t_command *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;

	if (!shell->is_interactive)
		dup2(2, 0);
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
	//exec_builtin(cmd, shell);
	//handle_signal_exit_status(shell);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
}
// function - 2
int	handle_command(t_shell *shell, char *line)
{
	t_command	*cmd;

	if (!line || line[0] == 0)
		return 0;
	cmd = parser(line, shell);
	print_command_list(cmd);
	
	if ((!cmd || !cmd->arg || cmd->arg[0] == 0) &&(cmd->redir_count < 0))
        return (0);
	if ((!cmd || !cmd->arg || cmd->arg[0] == 0) && (cmd->redir_count > 0))
	{
		maybe_preprocess_heredocs(cmd);
		run_with_redirection(cmd, shell);
		return(1);
	}
	if (cmd->next || cmd->is_pipe)
		execute_pipeline(cmd, shell);
	else
		execute_cmd(cmd, shell);
	free_cmd(cmd);
	return (1);
}

// function - 3
int interactive(t_shell *shell, char **command_line)
{
	(void)shell;
	// write(STDOUT_FILENO, "\n", 1);
	*command_line = readline("minishell$ ");
	if (*command_line == NULL)
	{
		// Check if this is due to a signal interruption
		// if (g_signal_status != 0)
		// {
		// 	// Signal was handled, continue the loop
		// 	return 2; // Continue loop without processing command
		// }
		// // Genuine EOF (Ctrl+D)
		return 0; // Break loozp
	}
	if (**command_line)
		add_history(*command_line);
	else
		return 2;
	return 1;
}

// function - 4
int non_interactive(t_shell *shell, char **command_line)
{
	*command_line = readline_non_interactive(STDIN_FILENO);
	if (!*command_line)
	{
		shell->last_exit_status = 0;
		return 0; // Break loop
	}
	if (!**command_line) // Handles empty lines from script
	{
		free(*command_line);
		return 2; // Continue loop
	}
	return 1; // Success
}


// function - 6
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int stat;

	shell = create_shell(argc, argv, envp);
	if (!shell)
		return (1);
	if (shell->is_interactive)
	{
		if (setup_interactive_signals() == -1)
		{
			free(shell);
			exit(EXIT_FAILURE);
		}
		greets_minishell();
	}
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
