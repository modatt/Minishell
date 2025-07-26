#include "minishell.h"

// function - 1
void exec_external(t_command *cmd, t_shell *shell)
{
	char *resolved_path;
	pid_t pid;
	int status;

	resolved_path = get_resolved_path(cmd, shell);
	if(!resolved_path)
		return ;
	pid = fork();	
	if(pid < 0)
	{
		shell->last_exit_status = 1;
		free(resolved_path);
		return ;
	}
	else if(pid == 0)
	{
        if (cmd->redir_count > 0)
			setup_redirection_fds(cmd);
		execve(resolved_path, cmd->arg, shell->envp);
		free(resolved_path);
		clean_exit(shell, 127, cmd);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		shell->last_exit_status = 128 + WTERMSIG(status);
}
/*
with comments:
// Wait for the child process with PID 'pid' to finish execution.
// 'status' will hold information about how the child terminated.
waitpid(pid, &status, 0);

// Check if the child process terminated normally (by calling exit or returning from main).
if (WIFEXITED(status))
    // If yes, extract the exit status code (the value passed to exit or returned).
    shell->last_exit_status = WEXITSTATUS(status);

// Check if the child process was terminated by a signal (e.g., Ctrl+C, kill).
if (WIFSIGNALED(status))
    // If yes, set the exit status to 128 + signal number (this is the standard convention in shells).
    shell->last_exit_status = 128 + WTERMSIG(status);

*/

