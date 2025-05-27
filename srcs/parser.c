#include "minishell.h"

/* parsing steps::
    1. tokenization:
        split string into words "edge case: qoutes"
        char **tokens;
    2. split commands:
        by operators
    3. add to data struct: 

*/
// main parsing function 

void    parser(char *command_line)
{
    char **tokens; 
    // t_command   *command;
    int k;

    k = 0;
    // 1) tokenizing: split the command_line.
    tokens = tokenizer(command_line);

    // init of the struct 
    // init_struct(command);
    // 2) split tokens into commands by operators
    while (tokens[k])
    {
        // find operator 
        // if (is_operator(tokens[k], command) == 1) // 
        // {
        //     // split 
        //     command.arg[i] = tokens[k];
        // }
        printf("%s\n", tokens[k]);
        k++;
    }
}

// psudocode:: 1) tokenization. 
/*
// check if line == NULL
    // find wc
    // init a char **tokens & allocate it using "ft_wc"
        // check malloc failer 
    // split the command into words.
    // loop inside the command line - string 
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
                    // extra the $ handling
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
                while (str[i] != 34)
                    i++;
                i++;
                wc++;
            }
            if (str[i] == 39)
            {
                i++;
                while (str[i] != 39)
                    i++;
                i++;
                wc++;
            }
            if ((str[i] != 34 && str[i] != 39) && (str[i] != ' ' || str[i] != '\t'))
                wc++;
            i++;
        }
        while (str[i] && !((str[i] == ' ' || str[i] == '\t')))
            i++;
    }
    printf("%d\n", wc);
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
    wc = count_words(line);
    tokens = (char**)malloc(sizeof(char *) * (wc + 1));
    if (!tokens)
        return (NULL);
    // error_handle();
    while (line[i])
    {
        // skip whitespaces
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        wbeg = i;
        while (line[i])
        {
            // check for double qoute
            if (line[i] == 34)
             {
                i++;
                tokens[k] = (char *)malloc(sizeof(char) * (i - wbeg + 1));
                ft_strncpy(tokens[k++], &line[wbeg], (i - wbeg));
            }
            if (line[i] == 39)
                // double_qoute(&line[i]);
            while (line[i] && !((line[i] == ' ' && line[i] == '\t')))
                i++;
            if (wbeg < i)
            {
                tokens[k] = (char *)malloc(sizeof(char) * (i - wbeg + 1));
                ft_strncpy(tokens[k++], &line[wbeg], (i - wbeg));
            }
        }
    }
    tokens[k] = NULL;
    return (tokens);
}



char    *ft_strncpy(char *des, char *src, int n)
{
    int i = -1;

    while (++i < n && src[i])
        des[i] = src[i];
    des[i] = '\0';
    return des;
}
