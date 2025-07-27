/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/07/27 19:45:28 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int main(int argc, char **argv, char **envp)
{
    greets_minishell();  // Optional greeting banner

    char *command_line;
    t_command *cmd;

    t_shell *shell = malloc(sizeof(t_shell));
    if (!shell)
    {
        perror("Failed to allocate shell structure");
        return (1);  // Handle memory allocation failure
    }

    init_shell(shell, argc, argv, envp);      // Set argc, argv, etc. (no envp)
    if (!shell->envp)
    {
        printf("Warning: Failed to initialize environment\n");
        // Continue anyway with empty environment
    }
            // Convert envp >> shell->env_list

    // if (signals_handling() == -1)
    // {
    //     free_shell(shell);  // Clean up shell if signal setup fails
    //     return -1;
    // }
	signals_prompt();
    while (1)
    {
		g_heredoc_interrupted = 0;
		if(!isatty(0))
			dup2(2, 0);
        command_line = readline("minishell$ ");
        if (command_line == NULL)
        {
            printf("exit\n");  // Print exit message for Ctrl+D
            break; // Exit gracefully on Ctrl+D
        }
        if (*command_line)
            add_history(command_line);
        // Skip empty commands but don't leak memory
        if (!*command_line)
        {
            free(command_line);
            continue;
        }
        cmd = parser(command_line, shell);
        if (!cmd)
        {
            free(command_line);
            continue;  // Skip if parsing failed
        }
        print_command_list(cmd);  // For debugging
        // Check if we have a pipeline (multiple commands or is_pipe flag set)
        if (cmd && (cmd->next != NULL || cmd->is_pipe))
            execute_pipeline(cmd, shell);
        else if (cmd)
            execute_cmd(cmd, shell);
        free_cmd(cmd);  // Free command list after execution
        free(command_line);
    }
    // Cleanup shell resources before exit
    clear_history();  // Clear readline history
    free_shell(shell);
    return (0);
}


