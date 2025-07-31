/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/07/31 15:13:02 by modat            ###   ########.fr       */
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

// function - 2
static int	handle_command(t_shell *shell, char *line)
{
	t_command	*cmd;

	cmd = parser(line, shell);
	if (!cmd)
		return (0);
	//print_command_list(cmd);
	if (cmd->next || cmd->is_pipe)
		execute_pipeline(cmd, shell);
	else
		execute_cmd(cmd, shell);
	free_cmd(cmd);
	return (1);
}

// function - 3
static int interactive(t_shell *shell, char **command_line)
{
	(void)shell;
	*command_line = readline("minishell$ ");
	if (!*command_line)
	{
		printf("exit\n");
		return 0; // Break loop
	}
	if (**command_line)
		add_history(*command_line);
	else
	{
		free(*command_line);
		return 2; // Continue loop
	}
	return 1; // Success
}

// function - 4
static int non_interactive(t_shell *shell, char **command_line)
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

// function - 5
void main_loop(t_shell *shell)
{
	char *command_line;
	int status;

	while (1)
	{
		shell->heredoc_interrupted = 0;
		if (shell->is_interactive)
			status = interactive(shell, &command_line);
		else
			status = non_interactive(shell, &command_line);
		if (status == 0)
			break;
		if (status == 2)
			continue;
		if (!handle_command(shell, command_line))
		{
			free(command_line);
			continue;
		}
		free(command_line);
	}
}


// function - 6
int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	shell = create_shell(argc, argv, envp);
	if (!shell)
	return (1);
	if (shell->is_interactive)
	{
		signals_prompt();
		greets_minishell();
	}
	else 
		signals_non_interactive();
	main_loop(shell);
	free_shell(shell);
	clear_history();
	return (shell->last_exit_status);
}
