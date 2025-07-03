/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:53:47 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/07/03 15:12:10 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//Command 'cdd' not found, did you mean: - Hala 

void	 execute_cmd(t_command *cmd, t_shell *shell)
{
	if (!cmd || !cmd->arg)
		return ; 
	// if(cmd->is_pipe == true)
	// {
    	if (is_builtin(cmd->arg[0]))
        	exec_builtin(cmd, shell);
    	//else
		////exec_external(cmd, shell);
		else
		{
			write(2, "Command '", 9);
			write(2, cmd->arg[0], ft_strlen(cmd->arg[0]));
			write(2, "' not found\n",12);
		}
		return ;
	//}
	//else
	//{
	//	//  PIPE_HANDLING
	//	return
	//}
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
