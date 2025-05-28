// #include "minishell.h"

#include <stdio.h>
#include <stdlib.h>

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
            if (str[i] == 34)
            {
                i++;
                while (str[i] && str[i] != 34)
                    i++;
                i++;
                wc++;
            }
            if (str[i] == 39)
            {
                i++;
                while (str[i] && str[i] != 39)
                    i++;
                i++;
                wc++;
            }
            while (str[i] && (str[i] == ' ' || str[i] == '\t'))
                i++;   
        }
        if (str[i])
            wc++;
        while (str[i] && !((str[i] == ' ' || str[i] == '\t')))
            i++;
    }
    printf("%d\n", wc);
    return (wc);
}

char    *ft_strncpy(char *des, char *src, int n)
{
    int i = -1;

    while (++i < n && src[i])
        des[i] = src[i];
    des[i] = '\0';
    return des;
}


// char**    parsing(char *str)
// {
//     char **split;
//     split = ft_split(str, ' ');
//     if (!split)
//         exit (-1);
//         //exit from the whole program
//     return (split);
// }


// char    **tokenizer(char *line)
// {
//     if (!line)
//         return (NULL);
//     int i;
//     int k = 0;
//     char **tokens;
//     int wc;
//     int wbeg;
//     int end = 0;

//     i = 0;
//     wc = count_words(line);
//     tokens = (char**)malloc(sizeof(char *) * (wc + 1));
//     if (!tokens)
//         return (NULL);
//     // error_handle();
//     while (line[i])
//     {
//         // skip whitespaces
//         while (line[i] == ' ' || line[i] == '\t')
//             i++;
//         wbeg = i;
//         while (line[i])
//         {
//             if (line[i] == 34) // double quote
//             {
//                 i++;                // skip opening quote
//                 wbeg = i;           // mark beginning of actual content inside quotes
//                 while (line[i] && line[i] != 34) // go until closing quote or end of line
//                     i++;
//                 if (line[i] == 34) // closing quote found
//                 {
//                     end = i;   // end is just before the closing quote
//                     tokens[k] = (char *)malloc(sizeof(char) * (end - wbeg + 1));
//                     ft_strncpy(tokens[k++], &line[wbeg], end - wbeg);
//                     i++; // skip closing quote
//                 }
//                 else
//                 {
//                     // error: unmatched quote
//                     // handle as you prefer (skip, copy till end, or print error)
//                 }
//             }           
//             if (line[i] == 39)
//             {
//                 i++;
//                 while (line[i] && line[i] != 39)
//                     i++;
//                 end = i;
//                 tokens[k] = (char *)malloc(sizeof(char) * (end - wbeg + 1));
//                 ft_strncpy(tokens[k++], &line[wbeg], (end - wbeg));
//                 wbeg = end;
//             }
//             while (line[i] && ((line[i] != ' ' || line[i] != '\t')))
//                 i++;
//             if (wbeg < i)
//             {
//                 tokens[k] = (char *)malloc(sizeof(char) * (i - wbeg + 1));
//                 ft_strncpy(tokens[k++], &line[wbeg], (i - wbeg));
//             }
//         }
//     }
//     tokens[k] = NULL; 
//     return (tokens);
// }

char **tokenizer(char *line)
{
    if (!line)
        return NULL;

    int i = 0, k = 0;
    int wbeg, end;
    int wc = count_words(line);
    char **tokens = malloc(sizeof(char *) * (wc + 1));

    if (!tokens)
        return NULL;

    while (line[i])
    {
        // Skip whitespace
        while (line[i] == ' ' || line[i] == '\t')
            i++;

        if (line[i] == 34) // double quote
        {
            i++;            // skip opening quote
            wbeg = i;       // start of content
            while (line[i] && line[i] != 34)
                i++;
            end = i;        // index of closing quote
            tokens[k] = malloc(end - wbeg + 1);
            ft_strncpy(tokens[k++], &line[wbeg], end - wbeg);
            if (line[i])    // skip closing quote
                i++;
            continue;
        }

        if (line[i] == 39) // single quote
        {
            i++;            // skip opening quote
            wbeg = i;
            while (line[i] && line[i] != 39)
                i++;
            end = i;
            tokens[k] = malloc(end - wbeg + 1);
            ft_strncpy(tokens[k++], &line[wbeg], end - wbeg);
            if (line[i])    // skip closing quote
                i++;
            continue;
        }

        if (line[i]) // regular word
        {
            wbeg = i;
            while (line[i] && line[i] != ' ' && line[i] != '\t'
                   && line[i] != 34 && line[i] != 39)
                i++;
            end = i;
            tokens[k] = malloc(end - wbeg + 1);
            ft_strncpy(tokens[k++], &line[wbeg], end - wbeg);
        }
    }
    tokens[k] = NULL;
    return tokens;
}


int     main(void)
{
    char *str = "test 2 3 4 5 " ;
    char **d = NULL;
    int i = 0;

    d = tokenizer(str);
    while (d[i])
    {
        printf("Token %d: [%s]\n", i, d[i]);
        i++;
    }
    return (0);
}



// char    **tokenizer(char *line)
/*{
    // check if line == NULL
    // find wc
    // init a char **tokens & allocate it using "ft_wc"
        // check malloc failer 
    // split the command into words.
    // loop inside the command line - string 
        // skip whitespaces
        // wbeg after whitespaces
        //  qoute case
            // sigle 
                {   
                    // skip the qoute char 
                    // mark the start of word
                    // skip the word 
                    // mark end - not sure
                    // copy word
                    // skip the end qoute
                    // mark new wbeg
                }
                else 
                    {
                        // incase of an error "one qoute"
                    } 
            // double
                {   
                    same as single qoute
                }
                else 
                    {
                        // incase of an error "one qoute"
                    } 
            // normal word spreated by a space
                // loop through string && condition that its not a whitespaces
                    //skip the word
                // if wbeg < i 
                    // copy the word

            // tokens[k] = NULL;
            // rteurn (tokens)

    

}*/