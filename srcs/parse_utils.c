/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:25:55 by modat             #+#    #+#             */
/*   Updated: 2025/05/31 20:25:58 by modat            ###   ########.fr       */
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

