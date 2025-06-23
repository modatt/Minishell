/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:25:55 by modat             #+#    #+#             */
/*   Updated: 2025/06/23 00:28:03 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// main function - 1
char    **tokenizer(char *line)
{
    if (!line)
        return (NULL);
    int i;
    int k;
    char **tokens;
    int wc;

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
        tokenizer2(tokens, &k, &i, line);
    }
    tokens[k] = NULL;
    return (tokens);
}


// function - 2
void    tokenizer2(char **tokens, int *k, int *i, char *line)
{
    int     wbeg; 
     while (line[*i])
        {
            while (line[*i] == ' ' || line[*i] == '\t')
                (*i)++;
            if (line[*i] == 34)
             {
                handle_double_qoute(tokens, k, line, i, &wbeg);
            }
            if (line[*i] == 39)
            {
                handle_single_qoute(tokens, k, line, i, &wbeg);
            }
            while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
               (*i)++; 
            while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
                (*i)++;
            
            if (line[*i])
            {
                handle_word(tokens, k, line, i, &wbeg);
            }
        }
}


// function - 3
void    handle_word(char **tokens, int *k, char *line, int *i, int *wbeg)
{
    *wbeg = *i;
    while (line[*i] && ((line[*i] != ' ' && line[*i] != '\t')))
        (*i)++;
    if (*wbeg < *i)
    {
        tokens[*k] = (char *)malloc(sizeof(char) * (*i - *wbeg + 1));
        if (!tokens[*k])
            return ;
        ft_strncpy(tokens[*k], &line[*wbeg], (*i - *wbeg));
        (*k)++;
    }
}

// function - 3
void    handle_double_qoute(char **tokens, int *k, char *line, int *i, int *wbeg)
{
                (*i)++;
                *wbeg = *i - 1;
                while (line[*i] && line[*i] != 34)
                    (*i)++;
                while (line[*i] && line[*i] != '|' && line[*i] != ' ')
                    (*i)++;
                (*i)++;
                tokens[*k] = (char *)malloc(sizeof(char) * (*i - *wbeg + 1));
                if (!tokens[*k])
                    return ;
                    // error message + exit program & free **tokens and what is before it if is init like struct & so on 
                ft_strncpy(tokens[*k], &line[*wbeg], (*i - *wbeg));
                (*k)++;
                (*i)++;
                *wbeg = *i + 1;
}

// function - 4
void    handle_single_qoute(char **tokens, int *k, char *line, int *i, int *wbeg)
{
                (*i)++;
                *wbeg = *i - 1;
                while (line[*i] && line[*i] != 39)
                    (*i)++;
                tokens[*k] = (char *)malloc(sizeof(char) * (*i - *wbeg + 1));
                if (!tokens[*k])
                    return ;
                    // error message + exit program & free **tokens and what is before it if is init like struct & so on
                ft_strncpy(tokens[*k], &line[*wbeg], (*i + 1 - *wbeg));
                (*k)++;
                (*i)++;
                *wbeg = *i + 1; 
}

// function - 5

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