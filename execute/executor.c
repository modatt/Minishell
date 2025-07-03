/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:53:47 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/07/03 13:18:14 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_builtin(char *cmd)
{
	// if (cmd == NULL)
	// 	return EXIT_FAILUAR; 
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
void 	builtin_env(t_shell *shell)
{
	t_env_var *current;

	current = shell->env_list;
	while(current)
	{
		if (current->value)
			printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}
//PATH_MAX = is a constant defiend in <limits.h> that represnt the max num of char in a full absulat file path 
// we need a buffer to store the current path
void  builtin_pwd(t_command *cmd)
{
		(void)cmd;	
		char path[PATH_MAX + 1];
		
		if(getcwd(path, sizeof(path)))
			printf("%s \n", path);
		return ;
}

void	builtin_echo(t_command *cmd)
{
	int	i;
	if(!cmd->arg[1])
	{
		printf("\n");
		return ;
	}
	i = 1;
	while (cmd->arg[i] && (ft_strcmp(cmd->arg[i],"-n") == 0))
		i++;
	while (cmd->arg[i])
	{
		printf("%s", cmd->arg[i]);
		i++;
		if(cmd->arg[i])
			printf(" ");
	}
	if (ft_strcmp(cmd->arg[1],"-n"))
		printf("\n");
}

int exec_builtin(t_command *cmd, t_shell *shell)
{
	if (!ft_strcmp(cmd->arg[0], "pwd"))
			builtin_pwd(cmd);
	if (!ft_strcmp(cmd->arg[0], "echo"))
			builtin_echo(cmd);
	if (!ft_strcmp(cmd->arg[0], "env"))
			builtin_env(shell);
    // if (!ft_strcmp(cmd->arg[0], "cd"))
    //    	builtin_cd(cmd, shell);
	if (!ft_strcmp(cmd->arg[0], "export"))
       	builtin_export(cmd, shell);
    if (!ft_strcmp(cmd->arg[0], "unset"))
       	builtin_unset(cmd, shell);
    // if (!ft_strcmp(cmd->arg[0], "exit"))
    //    	builtin_exit((cmd->arg[0]), shell);
    return (1);
}


void execute_cmd(t_command *cmd, t_shell *shell)
{
	if (!cmd || !cmd->arg) // without it cased sigfault and exit the program when hitting enter 
		return ;
    if (is_builtin(cmd->arg[0]))
        exec_builtin(cmd, shell);
    else
		return ;
        // exec_external(cmd, shell);
}
