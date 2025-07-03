/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/07/03 13:01:07 by modat            ###   ########.fr       */
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
        return (1);  // Handle memory allocation failure

    init_shell(shell, argc, argv, envp);      // Set argc, argv, etc. (no envp)
            // Convert envp >> shell->env_list

    if (signals_handling() == -1)
        return -1;

    while (1)
    {
        command_line = readline("minishell$ ");
        if (command_line == NULL)
            break; // Exit gracefully on Ctrl+D

        if (*command_line)
            add_history(command_line);

        cmd = parser(command_line, shell);
        print_cmd_list(cmd);  // For debugging

        execute_cmd(cmd, shell);

        // free_command_list(cmd);  // If you implemented it
        free(command_line);
    }

    // Cleanup (env_list, etc.)
    // free_env_list(shell->env_list);
    free(shell);
    return (0);
}


