/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/06/25 19:29:48 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int main(int argc, char **argv, char **envp)
{
    greets_minishell();
    char *command_line;
    t_command *list;
    t_shell *shell = NULL;
    
    allocate_memory_shell(&shell);
    init_shell(shell, argc, argv, envp);
    
    if (signals_handling() == -1)
        return -1;  
        
    while (1)
    {
        command_line = readline("minishell$ ");
        if (command_line == NULL)
            exit (-1);
        if (*command_line) //* if the first character not '\0' then the string isn't empty
            add_history(command_line);
        list = parser(command_line, shell);
        print_cmd_list(list);
    // Managing memory and cleaning up resources
        // free_list()
    }
    // free_cmd_list();
    // free_tokens();
    free(command_line);
    return 0;
}

