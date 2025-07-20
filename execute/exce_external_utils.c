#include "minishell.h"

// function --- 1
//fork is also a system call, we should check signals in the child process
 void print_command_not_found(t_command *cmd, t_shell *shell)
{
	write(2, "Command '", 9);
	write(2, cmd->arg[0], ft_strlen(cmd->arg[0]));
	write(2, "' not found\n", 12);
	shell->last_exit_status = 127;
	return ;
}

// function --- 2
// Helper: join a directory and a command into dir/cmd
char *join_path_cmd(const char *dir, const char *cmd)
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

// function --- 3
char **get_path_dirs(t_shell *shell)
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

// function --- 4
 char *find_cmd_in_path(char *arg, t_shell *shell)
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
// function --- 5
 char *get_resolved_path(t_command *cmd, t_shell *shell)
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

