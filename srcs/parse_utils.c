/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:25:55 by modat             #+#    #+#             */
/*   Updated: 2025/06/18 13:43:30 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



char    **tokenizer(char *line)
{
    if (!line)
        return (NULL);
    int i;
    int k;
    char **tokens;
    int wc;
    int wbeg;
    t_qoute     double_single;

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
            if (line[i] == '|')
            {
                free_tokens(tokens);
                write(1, "minishell: syntax error near unexpected token `|'\n", 50);
                exit(EXIT_FAILURE); 
            }
            // check for double qoute
            if (line[i] == 34)
             {
                double_single = DOUBLE;
                i++;
                wbeg = i;
                while (line[i] && line[i] != 34)
                    i++;
                while (line[i + 1] && line[i + 1] != '|' && line[i + 1] != ' ')
                    i++;
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
                double_single = SINGLE;
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



void    syntax_checker()
{
    // if ()
    // if (line[i] == only pipe)

    // if (line[i] == no cmd after a pipe)
}

void    free_tokens(char **tokens)
{
    if (!tokens)
        return ;
    int i; 

    i = 0;
    while (tokens[i])
    {
        free(tokens[i]);
        i++;
    }
    free(tokens);
}