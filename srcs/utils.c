/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:38 by modat             #+#    #+#             */
/*   Updated: 2025/05/30 09:21:16 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handler(int sig)
{
    // (void)sig;                 /* We don’t need the value itself            */
    write(1, "\n", 1);         /* 1. move the cursor to a new terminal line */
    if (sig == SIGINT )
    {
        rl_on_new_line();          /* 2. tell readline: “we’re on a new line”   */
        rl_replace_line("", 0);    /* 3. clear whatever the user was typing     */
        rl_redisplay();            /* 4. repaint the prompt + empty input line*/
    }
    // else if (sig == SIGQUIT)
    return ;
}


int     signals_handling(void)
{
    struct sigaction sig;
    sig.sa_handler = handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sig, NULL) == -1)
        return -1;
      // Set up SIGQUIT (Ctrl-\) to be ignored
    sig.sa_handler = SIG_IGN;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;
    if (sigaction(SIGQUIT, &sig, NULL) == -1)
        return -1;
    return 0;
}

char    *ft_strncpy(char *des, char *src, int n)
{
    int i = -1;

    while (++i < n && src[i])
        des[i] = src[i];
    des[i] = '\0';
    return des;
}


void    init_cmd(t_command *cmd)
{
    cmd->arg = NULL;
    cmd->is_pipe = false;
    cmd->redirection = NONE;
    cmd->file = NULL;
    cmd->index = 0;
    cmd->is_wildcard = false;
    cmd->next = NULL;
}
