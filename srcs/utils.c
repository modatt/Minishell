/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:38 by modat             #+#    #+#             */
/*   Updated: 2025/06/22 17:22:50 by modat            ###   ########.fr       */
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
    (cmd)->arg = NULL;
    (cmd)->is_pipe = false;
    // (cmd)->redirection = REDIR_NONE;
    // (cmd)->file = NULL;
    (cmd)->is_wildcard = false;
    (cmd)->next = NULL;
}

int     count_words(char *str)
{
    int i;
    int wc;

    i = 0;
    wc = 0;
    while (str[i])
    {
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        if (str[i] == '|')
        {
            while (str[i] == ' ' || str[i] == '\t')
                i++;
            if (str[i] == '|' || str[i + 1] == '\0' || str[i + 1] == '|' )
            {
                write(1, "minishell: syntax error near unexpected token `|'\n", 50);
                return 0;   
            }
        }
       if (str[i] == '>' || str[i] == '<')
        {
            while (str[i] == ' ' || str[i] == '\t')
                i++;
            if (str[i + 2] == '>' || str[i + 2] == '<')
            {
                // >><
                if (str[i + 1] == str[i])
                {
                    // >><<
                    if (str[i + 2] == str[i + 3])
                        printf("minishell: syntax error near unexpected token `%c%c'\n", str[i + 2], str[i + 3]);
                    else  // >><
                        printf("minishell: syntax error near unexpected token `%c'\n", str[i + 2]);
                    return 0;
                }
                else 
                {
                    if (str[i + 2] == str[i + 1])
                        printf("minishell: syntax error near unexpected token `%c%c'\n", str[i + 2], str[i + 1]);
                    else  // >><
                        printf("minishell: syntax error near unexpected token `%c'\n", str[i + 2]);
                    return 0;
                }
            }
           if (str[i] == '>' || str[i] == '<')
           {
                if (str[i + 1] == str[i])
                {
                    while (str[i] == ' ' || str[i] == '\t')
                        i++;
                    if (str[i] != '\0')
                    {
                        wc++;
                        while (str[i] && !((str[i] == ' ' || str[i] == '\t')))
                            i++;
                    }
                }
           }
            if (str[i + 1] == '\0')
            {
                write(1, "bash: syntax error near unexpected token `newline'\n", 51);
                return 0;  
            } 
        }          
        if (str[i])
        {
          
           if (str[i] && str[i] == 34)
            {
                i++;
                while (str[i] && str[i] != 34)
                    i++;
                i++;
                wc++;
            }
            if (str[i] && str[i] == 39)
            {
                i++;
                while (str[i] && str[i] != 39)
                    i++;
                i++;
                wc++;
            }
            while (str[i] && (str[i] == ' ' || str[i] == '\t'))
                i++;
            if (str[i] && (str[i] != 34 && str[i] != 39) && (str[i] != ' ' || str[i] != '\t'))
            {   
                if ((str[i] == '>' || str[i] == '<') && wc == 0)
                {
                    while (str[i] == ' ' || str[i] == '\t')
                        i++;
                    if (str[i + 1] == '|' || str[i + 1] == '\0' || (str[i + 1] == '>' || str[i + 1] == '<'))
                    {
                        write(1, "minishell: syntax error near unexpected token `newline'\n", 56);
                        return 0;  
                    }
                    if (str[i + 2] == '>' || str[i + 2] == '<')
                    {
                        printf("minishell: syntax error near unexpected token `%c'\n", str[i + 2]);
                        return 0;
                    }
                }
                while (str[i] && !((str[i] == ' ' || str[i] == '\t' || str[i] == ')')))
                    i++;
                wc++;
            }
        }    
    }
    return (wc);
}

char    *ft_strcpy(char *des, char *src)
{
    if (!src)
        return NULL;
    int i;
    
    i = 0;
    while (src[i])
    {
        des[i] = src[i];
        i++;
    }
    des[i] = '\0';
    return (des);
}


int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

