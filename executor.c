/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:53:47 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/06/25 17:21:11 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_builtin(char *cmd)
{
    const char *builtins[];
    int i;

    builtins[] = {"cd", "echo", "pwd", "export", "unset", "env", "exit"};
    i = 0;
    while(builtins[i])
    {
        if(!ft_strcmp(cmd, builtins[i]))
            return(true)
        i++;
    }
    return(false);
}

int exec_builtin(t_command cmd, t_shell shell)
{
    if (!ft_strcmp(cmd->arg[0], "cd"))
        	builtin_cd(cmd->args, shell->envp);
    if (!ft_strcmp(cmd->arg[0], "echo"))
        	builtin_echo(cmd->args, shell->envp);
    if (!ft_strcmp(cmd->cmd, "pwd"))
        	builtin_pwd(cmd->args, shell->envp);
    if (!ft_strcmp(cmd->arg[0], "export"))
        	builtin_export(cmd->args, shell->envp);
    if (!ft_strcmp(cmd->arg[0], "unset"))
        	builtin_unset(cmd->args, shell->envp);
    if (!ft_strcmp(cmd->arg[0], "env"))
        	builtin_env(cmd->argc, shell->envp);
    if (!ft_strcmp(cmd->arg[0], "exit"))
        	builtin_exit(cmd->args), shell->envp);
    return (1);
}


void execute_cmd(t_command *cmd, t_shell *shell)
{
    if (is_builtin(cmd->arg[0]))
        exec_builtin(cmd);
    else
        exec_external(cmd, envp);
}
