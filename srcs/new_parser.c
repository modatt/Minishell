#include "minishell.h"

char    **new_parser(char *command_line)
{
    char **tokens;
    //t_command *cmd;

    tokens = ft_split(command_line, '|');
    if (!tokens)
        return NULL;

    // if (validation(tokens) == true) // including error_mess() in case of failer.
    //     struct_filling(tokens, cmd);
    // else 
    // {
    //     free_memory();
    //     exit_program();
    // }
    return(tokens);
}




