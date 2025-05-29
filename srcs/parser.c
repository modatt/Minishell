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
    // all is done 

    // init of the struct 
    // init_struct(command);
    // 2) split tokens into commands by operators
    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return ;
    while (tokens[k])
    {
        if (ft_strcmp(tokens[k], "|") == 0)
        {
            create_cmd(cmd);
            if (redircor == true)
                
        }
        printf("%s\n", tokens[k]);
        k++;
    }
    // i think should be in a loop
    // free(tokens);
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
