#include "minishell.h"

// function --- 1
//fork is also a system call, we should check signals in the child process
static void print_command_not_found(t_command *cmd, t_shell *shell)
{
	write(2, "Command '", 9);
	write(2, cmd->arg[0], ft_strlen(cmd->arg[0]));
	write(2, "' not found\n", 12);
	shell->last_exit_status = 127;
	return ;
}

// function --- 2

// Helper: join a directory and a command into dir/cmd
static char *join_path_cmd(const char *dir, const char *cmd)
{
    char *tmp;
    char *full_path;

    // First we join "/" to the path
    tmp = ft_strjoin(dir, "/");
    if (!tmp)
        return (NULL);

	//now we join it to look like this :usr/bin/ls
    full_path = ft_strjoin(tmp, cmd);

    free(tmp);
	//free(cmd);
    return (full_path);
}

static char **get_path_dirs(t_shell *shell)
{
	char	*path_env;
	char	**dirs;

	path_env = get_envp(shell->envp, "PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	free(path_env);
	return (dirs);
}

// function --- 3
static char *find_cmd_in_path(char *arg, t_shell *shell)
{
	char	**dirs;
	char	*full_path;
	int		i;

	if (!(dirs = get_path_dirs(shell)))
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full_path = join_path_cmd(dirs[i], arg);
		if (!full_path)
		{
			free_env_copy(dirs);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free_env_copy(dirs);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_env_copy(dirs);
	return (NULL);
}





// function --- 4
static char *get_resolved_path(t_command *cmd, t_shell *shell)
{
	char *resolved_path;
	
	if(ft_strchr(cmd->arg[0], '/'))
		resolved_path = ft_strdup(cmd->arg[0]);
	else
		resolved_path = find_cmd_in_path(cmd->arg[0], shell);
	if(!resolved_path)
	{
		print_command_not_found(cmd, shell);
		return(NULL);
	}
	return(resolved_path);
}

// function - 5
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