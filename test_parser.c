#include "includes/minishell.h"
#include <stdio.h>

int main()
{
    t_shell shell;
    t_command *cmd;
    char *test_commands[] = {
        "ls",
        "echo hello",
        "cat file.txt",
        "ls | grep test",
        "cat < input.txt",
        "echo hello > output.txt",
        "ls -la",
        NULL
    };
    
    // Initialize shell
    shell.argc = 0;
    shell.argv = NULL;
    shell.last_exit_status = 0;
    shell.envp = NULL;
    shell.env_list = NULL;
    
    printf("Testing parser functions:\n");
    printf("========================\n\n");
    
    for (int i = 0; test_commands[i]; i++)
    {
        printf("Testing command: '%s'\n", test_commands[i]);
        
        // Test syntax checking first
        int word_count = count_words(test_commands[i]);
        printf("  Word count: %d\n", word_count);
        
        if (word_count > 0)
        {
            // Test parsing
            cmd = parser(test_commands[i], &shell);
            if (cmd)
            {
                printf("  Parsing: SUCCESS\n");
                printf("  First argument: %s\n", cmd->arg ? cmd->arg[0] : "NULL");
                printf("  Has pipe: %s\n", cmd->is_pipe ? "YES" : "NO");
                // Free the command structure
                free_cmd(cmd);
            }
            else
            {
                printf("  Parsing: FAILED\n");
            }
        }
        else
        {
            printf("  Syntax check: FAILED\n");
        }
        printf("\n");
    }
    
    return 0;
}
