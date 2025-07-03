#include "minishell.h"




bool 	is_builtin(char *cmd)
{
    int		i;
	char *builtins[] = {"cd", "echo", "pwd", "export", "unset", "env", "exit", NULL};
	
	i = 0;
    while (builtins[i])
    {
		if (!ft_strcmp(cmd, builtins[i]))
		return (true);
        i++;
    }
    return (false);
}


int exec_builtin(t_command *cmd, t_shell *shell)
{
	if (!ft_strcmp(cmd->arg[0], "pwd"))
			builtin_pwd(cmd);
	else if (!ft_strcmp(cmd->arg[0], "echo"))
			builtin_echo(cmd);
	else if (!ft_strcmp(cmd->arg[0], "env"))
			builtin_env(shell);
    // else if (!ft_strcmp(cmd->arg[0], "cd"))
    //    	builtin_cd(cmd, shell);
	else if (!ft_strcmp(cmd->arg[0], "export"))
       	builtin_export(cmd, shell);
    else if (!ft_strcmp(cmd->arg[0], "unset"))
       	builtin_unset(cmd, shell);
    // else if (!ft_strcmp(cmd->arg[0], "exit"))
    //    	builtin_exit(cmd, shell);
    return (1);
}
