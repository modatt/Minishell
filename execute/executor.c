/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:53:47 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/07/26 17:13:59 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void run_builtin_with_redirection(t_command *cmd, t_shell *shell)
{
    // Save current standard fds (stdin, stdout, stderr)
    int saved_stdin  = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stderr = dup(STDERR_FILENO);

    if (saved_stdin < 0 || saved_stdout < 0 || saved_stderr < 0)
    {
        perror("dup");
        return;
    }
    // Apply redirections (dup2 for input/output/error files)
    setup_redirection_fds(cmd);
    // Execute the builtin command (runs in parent)
    exec_builtin(cmd, shell);
    // Restore original standard fds
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);
    // Close the saved fds
    close(saved_stdin);
    close(saved_stdout);
    close(saved_stderr);
}

//       int dup2(int oldfd, int newfd);
void execute_cmd(t_command *cmd, t_shell *shell)
{
    if (!cmd)
        return;
    
    maybe_preprocess_heredocs(cmd);
    
    // Check if command has arguments before accessing them
    if (!cmd->arg || !cmd->arg[0])
    {
        // Command with only redirections, no actual command to execute
        return;
    }
    
    if (is_builtin(cmd->arg[0]))
    {
        if (cmd->redir_count > 0)
            run_builtin_with_redirection(cmd, shell);
        else
            exec_builtin(cmd, shell);
    }
    else
		exec_external(cmd, shell);
 }




// void execute_cmd(t_command *cmd, t_shell *shell)
// {
// 	if (!cmd || !cmd->arg) // without it cased sigfault and exit the program when hitting enter 
// 		return ;
//     if (is_builtin(cmd->arg[0]))
//         exec_builtin(cmd, shell);
//     else
// 		return ;
//         // exec_external(cmd, shell);
// }
