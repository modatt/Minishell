/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:25 by modat             #+#    #+#             */
/*   Updated: 2025/05/30 09:21:22 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* parsing steps::
    1. tokenization: "enhancment"
        split string into words "edge case: qoutes"
        char **tokens;
    2. split commands: "need to be done"
        by operators
    3. add to data struct: 

*/


// main parsing function 

void    parser(char *command_line)
{
    char **tokens;
    t_command *cmd;
    int k;

    k = 0;

    // 1) tokenizing: split the command_line.
    tokens = tokenizer(command_line);
    
    // 2) split tokens into commands by operators
    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return ;
    init_cmd(cmd);
    while (token[k])
    {
        split_command(tokens[k], &cmd);
        printf("%s\n", tokens[k]);
        k++;
    }
    // free(tokens);
}

void    split_command(char *tokens, t_command *cmd)
{
    int k;

    k = 0;
    // if not a pipe
    if (ft_strcmp(tokens[k], "|") == 1)
    {
        create_cmd();
    }
    
}

void    create_cmd();
{
    t_command   cmd
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
                wc++;
                while (str[i] && !((str[i] == ' ' || str[i] == '\t' || str[i] == ')')))
                    i++;
            }
        }
    
        
    }
    return (wc);
}



char    **tokenizer(char *line)
{
    if (!line)
        return (NULL);
    int i;
    int k;
    char **tokens;
    int wc;
    int wbeg;

    i = 0;
    k = 0;
    wc = count_words(line);
    tokens = (char**)malloc(sizeof(char *) * (wc + 1));
    if (!tokens)
        return (NULL);
    while (line[i] == ' ' || line[i] == '\t')
            i++;
    // error_handle();
    if (line[i])
    {
        while (line[i])
        {
            while (line[i] == ' ' || line[i] == '\t')
                i++;
            // check for double qoute
            if (line[i] == 34)
             {
                i++;
                wbeg = i;
                while (line[i] && line[i] != 34)
                    i++;
                tokens[k] = (char *)malloc(sizeof(char) * (i - wbeg + 1));
                if (!tokens)
                    return NULL;
                    // error message + exit program & free **tokens and what is before it if is init like struct & so on 
                ft_strncpy(tokens[k++], &line[wbeg], (i - wbeg));
                i++;
                wbeg = i;
            }
            if (line[i] == 39)
            {
                i++;
                wbeg = i;
                while (line[i] && line[i] != 39)
                    i++;
                tokens[k] = (char *)malloc(sizeof(char) * (i - wbeg + 1));
                if (!tokens)
                    return NULL;
                    // error message + exit program & free **tokens and what is before it if is init like struct & so on
                ft_strncpy(tokens[k++], &line[wbeg], (i - wbeg));
                i++;
                wbeg = i;  
            }
            while (line[i] && (line[i] == ' ' || line[i] == '\t'))
                i++; 
            while (line[i] && (line[i] == ' ' || line[i] == '\t'))
                i++;
            
            if (line[i])
            {
                wbeg = i;
                while (line[i] && ((line[i] != ' ' && line[i] != '\t')))
                    i++;
                if (wbeg < i)
                {
                    tokens[k] = (char *)malloc(sizeof(char) * (i - wbeg + 1));
                    if (!tokens)
                        return NULL;
                    ft_strncpy(tokens[k++], &line[wbeg], (i - wbeg));
                }
                // i++;
            }
        }
    }
        // error handle only spaces line
    tokens[k] = NULL;
    return (tokens);
}

