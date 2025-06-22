/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:25:55 by modat             #+#    #+#             */
/*   Updated: 2025/06/22 15:43:42 by modat            ###   ########.fr       */
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

    i = 0;
    k = 0;
    wc = count_words(line); // syntax error 
    if (wc == 0)
        return NULL;
    tokens = (char**)malloc(sizeof(char *) * (wc + 1));
    if (!tokens)
        return (NULL);
    while (line[i] == ' ' || line[i] == '\t')
            i++;
    if (line[i])
    {
        tokenizer2();
    }
    tokens[k] = NULL;
    return (tokens);
}


void    tokenizer2()
{
     while (line[i])
        {
            while (line[i] == ' ' || line[i] == '\t')
                i++;
            if (line[i] == 34)
             {
                i++;
                wbeg = i - 1;
                while (line[i] && line[i] != 34)
                    i++;
                while (line[i] && line[i] != '|' && line[i] != ' ')
                    i++;
                i++;
                tokens[k] = (char *)malloc(sizeof(char) * (i - wbeg + 1));
                if (!tokens)
                    return NULL;
                    // error message + exit program & free **tokens and what is before it if is init like struct & so on 
                ft_strncpy(tokens[k++], &line[wbeg], (i - wbeg));
                i++;
                wbeg = i + 1;
                // if (l == $)
            }
            if (line[i] == 39)
            {
                i++;
                wbeg = i - 1;
                while (line[i] && line[i] != 39)
                    i++;
                tokens[k] = (char *)malloc(sizeof(char) * (i - wbeg + 1));
                if (!tokens)
                    return NULL;
                    // error message + exit program & free **tokens and what is before it if is init like struct & so on
                ft_strncpy(tokens[k++], &line[wbeg], (i + 1 - wbeg));
                i++;
                wbeg = i + 1;  
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
            }
        }
}

// void    free_tokens(char **tokens)
// {
//     if (!tokens)
//         return ;
//     int i; 

//     i = 0;
//     while (tokens[i])
//     {
//         free(tokens[i]);
//         i++;
//     }
//     free(tokens);
// }