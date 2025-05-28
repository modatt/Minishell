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
    // t_command   **command;
    int k;
    // int i;
    // int operator_flag;

    k = 0;
    // i = 0;
    // operator_flag = 0;
    // 1) tokenizing: split the command_line.
    tokens = tokenizer(command_line);
    // all is done 

    // init of the struct 
    // init_struct(command);
    // 2) split tokens into commands by operators
    while (tokens[k])
    {
        // t_command   **command;
        // commadn->arg[i] = tokens[k];
        // // find operator 
        // if (detect_operator(tokens[k], command) != 1)
        // {
        //     if (k == 0)
        //     {
        //         ft_strcpy(command->arg[i], tokens[k]);
        //         i++;
        //         k++;
        //     }
        //     else 
        //     {
        //         command->arg[i] = ft_strjoin(tokens[k -1], tokens[k]);
        //         k++;
        //     }
        //     operator_flag = 0;   
        // }
        // else 
        // {
        //     if (detect_operator(token[k], command))
        // }
        printf("%s\n", tokens[k]);
        k++;
    }
    // i think should be in a loop
    // free(tokens);
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
            // while (str[i] && (str[i] == ' ' || str[i] == '\t'))
            //     i++;  
            // if (str[i] == '(')
            // {
            //     wc++;  
            //     i++;
            // }
            while (str[i] && (str[i] == ' ' || str[i] == '\t'))
                i++;   
        }
        if (str[i])
            wc++;
        while (str[i] && !((str[i] == ' ' || str[i] == '\t' || str[i] == ')')))
            i++;
        // if (str[i] == ')')
        // {
        //     wc++;
        //     i++;
        // }
    }
    printf("%d\n", wc); // later must be deleted
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
            // if (line[i] == '(')
            // {
            //     // printf("I am here\n");
            //     wbeg = i;
            //     i++;
            //     tokens[k] = (char *)malloc(sizeof(char) * (i - wbeg + 1));
            //     if (!tokens)
            //         return NULL;
            // //         // error message + exit program & free **tokens and what is before it if is init like struct & so on
            //     ft_strncpy(tokens[k++], &line[wbeg], (i - wbeg));
            //     printf("hello printf %s", tokens[k]);
            // }
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



char    *ft_strncpy(char *des, char *src, int n)
{
    int i = -1;

    while (++i < n && src[i])
        des[i] = src[i];
    des[i] = '\0';
    return des;
}
/*
int     detect_operator(char *token, t_command *command)
{
    // int i;

    // i = 0;
    if (ft_strcmp(token, "|") == 1)
    {
        command->operator = PIPE;
        return 1;
    }
    else if (ft_strncmp(token, "&&") == 1)
    {
        command->operator = AND;
        return 1;
    }
    else if (ft_strncmp(token, "||") == 1)
    {
        command->operator = OR;
        return 1;
    }
    else 
        command->operator = NONE;
    return 0;
}
*/
