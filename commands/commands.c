/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:42 by modat             #+#    #+#             */
/*   Updated: 2025/05/30 08:17:43 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    commands(char *str, int n)
{
    if (ft_strncmp(str, n) == "echo")
    {
	    command->command_name = CD;
        cd_exe();
    }
    if (dECHO)
    {
	    //handle expansion of $
        
	}
    if (minus_n == 1)
    {
	    //handle -n
    }
    if (exit_command() == 1)
    {
        rl_on_new_line();          /* 2. tell readline: “we’re on a new line”   */
        rl_replace_line("", 0);    /* 3. clear whatever the user was typing     */
        rl_redisplay();            /* 4. repaint the prompt + empty input line*/

        // exit & free allocated memory & distroy connection might be built 
        exit(1);

        /*
        Ends the current process (your shell session)

Cleans up resources:

Closes open files

Frees memory

Notifies the OS that the process has ended
        */
    }
}

