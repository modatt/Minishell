/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 17:53:47 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/07/17 18:33:50 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//Command 'cdd' not found, did you mean: - Hala 
void execute_cmd(t_command *cmd, t_shell *shell)
{
	int		ret;
    if (!cmd || !cmd->arg)
        return;

    if (is_builtin(cmd->arg[0]))
    {
        exec_builtin(cmd, shell);
    }
    else
    {
        ret = exec_external(cmd, shell);
        if (ret == 127) // 127 → command not found
        {
            write(2, "Command '", 9);
            write(2, cmd->arg[0], ft_strlen(cmd->arg[0]));
            write(2, "' not found\n", 12);
        }
        shell->last_exit_status = ret;
    }
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
