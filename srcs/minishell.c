#include "minishell.h"

int main(void)
{
    char *command_line;
    
    if (signals_handling() == -1)
        return -1;

    while (1)
    {
        command_line = readline("minishell$ ");
        if (command_line == NULL)
            exit (-1);
    
        if (*command_line) //* if the first character not '\0' then the string isn't empty
            add_history(command_line);
        parser(command_line);
        
        // executor(sds);
  
    }
    free(command_line);
    return 0;
}





