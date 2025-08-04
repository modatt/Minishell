/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exce_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:51:46 by modat             #+#    #+#             */
/*   Updated: 2025/08/03 17:52:56 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
void	print_command_not_found(t_command *cmd, t_shell *shell)
{
	write(2, "Command '", 9);
	write(2, cmd->arg[0], ft_strlen(cmd->arg[0]));
	write(2, "' not found\n", 12);
	shell->last_exit_status = 127;
	return ;
}

// function - 2_
static void parent_handle(pid_t pid, t_shell *shell, char *resolved_path, t_command *cmd)
{
    int status;
    
    (void)cmd; // Suppress unused parameter warning

    waitpid(pid, &status, 0);
    free(resolved_path); // clean up

    if (WIFEXITED(status))
        shell->last_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->last_exit_status = 128 + WTERMSIG(status);
        
    // Restore interactive signal handling after external command
    if (shell->is_interactive)
        setup_interactive_signals();
}

// function - 3
static pid_t fork_and_exec_child(t_command *cmd, t_shell *shell, char *resolved_path, sigset_t *old_mask)
{
	pid_t pid = fork();

	if (pid == 0)
	{
		unblock_fork_signals(old_mask);
		if (setup_child_signals() == -1)
			clean_exit(shell, 1, cmd);

		if (cmd->redir_count)
			setup_redirection_fds(cmd);

		execve(resolved_path, cmd->arg, shell->envp);
		perror("execve");
		free(resolved_path);
		clean_exit(shell, 127, cmd);
	}
	return pid;
}

// function - 4
static char *prepare_exec(t_command *cmd, t_shell *shell, sigset_t *old_mask)
{
	char *resolved_path;

	resolved_path = get_resolved_path(cmd, shell);
	if (!resolved_path)
	{
		print_command_not_found(cmd, shell);
		return NULL;
	}
	if (block_fork_signals(old_mask) == -1)
	{
		free(resolved_path);
		shell->last_exit_status = 1;
		return NULL;
	}
	return resolved_path;
}

// function - 5
void exec_external(t_command *cmd, t_shell *shell)
{
	char		*resolved_path;
	pid_t		pid;
	sigset_t	old_mask;

	resolved_path = prepare_exec(cmd, shell, &old_mask);
	if (!resolved_path)
		return;

	pid = fork_and_exec_child(cmd, shell, resolved_path, &old_mask);
	if (pid < 0)
	{
		unblock_fork_signals(&old_mask);
		free(resolved_path);
		shell->last_exit_status = 1;
		return;
	}
	unblock_fork_signals(&old_mask); // Restore signal mask in parent
	parent_handle(pid, shell, resolved_path, cmd);
}


// function - 3
// void	exec_external(t_command *cmd, t_shell *shell)
// {
// 	char	*resolved_path;
// 	pid_t	pid;
// 	sigset_t 	old_mask;

// 	resolved_path = get_resolved_path(cmd, shell);
// 	if (!resolved_path)
// 	{
// 		print_command_not_found(cmd, shell);	
// 		return ;
// 	}
// 	if (block_fork_signals(&old_mask) == -1)
// 	{
// 		free(resolved_path);
// 		shell->last_exit_status = 1;
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("fork");
// 		free(resolved_path);
// 		unlock_fork_signals(&old_mask);
// 		shell->last_exit_status = 1;
// 		return ;
// 	}
// 	if (pid == 0) // child
// 	{
// 		unlock_fork_signals(&old_mask);
// 		if (setup_child_signals() == -1)
// 			clean_exit(shell, 1, cmd);
// 		if (cmd->redir_count)
// 			setup_redirection_fds(cmd);
// 		execve(resolved_path, cmd->arg, shell->envp);
// 		perror("execve");
// 		free(resolved_path);
// 		clean_exit(shell, 127, cmd);
// 	}
// 	parent_handle(pid, shell, resolved_path, cmd);
// }
