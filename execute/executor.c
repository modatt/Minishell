/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:53:47 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/06/26 14:18:47 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "libft.h"
#include <stdio.h>
#include <stdbool.h>
#include "minishell.h"

//typedef struct s_shell
//{
//	int		argc;
//	char	**argv;
//	char	**envp;
//	int		last_exit_status;
//}	t_shell;

// int	ft_strcmp(char *s1, char *s2)
// {
// 	unsigned int	i;

// 	i = 0;
// 	while (s1[i] == s2[i] && s1[i] != '\0')
// 	{
// 		i++;
// 	}
// 	if (s1[i] == s2[i])
// 	{
// 		return (0);
// 	}
// 	else
// 	{
// 		return (s1[i] - s2[i]);
// 	}
// }
bool is_builtin(char *cmd)
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
void 	builtin_env(t_shell *shell)
{

	int	i;
	
	i = 0;
	while(shell->envp[i])
	{
		if(ft_strchr(shell->envp[i], '='))
			printf("%s \n", shell->envp[i]);
		i++;
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
    //if (!ft_strcmp(cmd->args[0], "cd"))
    //    	builtin_cd(cmd, shell);
    //    	builtin_export(cmd, shell);
    //if (!ft_strcmp(cmd->args[0], "unset"))
    //    	builtin_unset(cmd, shell);
    //if (!ft_strcmp(cmd->args[0], "exit"))
    //    	builtin_exit((cmd->args[0]), shell);
    return (1);
}


void execute_cmd(t_command *cmd, t_shell *shell)
{
    if (is_builtin(cmd->arg[0]))
        exec_builtin(cmd, shell);
    else
		return ;
        //exec_external(cmd, shell);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell		shell;
// 	t_command	cmd;

// 	shell.argc = argc;
// 	shell.argv = argv;
// 	shell.envp = copying_env(envp);
// 	shell.last_exit_status = 42;

// 	cmd.args = calloc(4, sizeof(char *));
// 	if (!cmd.args)
// 		return (1);
// 	cmd.args[0] = ft_strdup("echo");
// 	//cmd.args[1] = ft_strdup("-n");
// 	cmd.args[1] = ft_strdup("-n-n");
// 	cmd.args[2] = ft_strdup("hello");
// 	cmd.args[3] = NULL;


// 	execute_cmd(&cmd, &shell);

// 	//free(cmd.args[0]);
// 	free(cmd.args);
	
// 	int i = 0;
// 	while (shell.envp[i])
// 		free(shell.envp[i++]);
// 	free(shell.envp);

// 	return (0);
// }

