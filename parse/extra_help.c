#include "minishell.h"


void print_cmd_list(t_command *head)
{
    int i;
    int node_index = 0;

    while (head)
    {
        printf("------ Command %d ------\n", node_index);

        // Print arguments
        i = 0;
        printf("Arguments: ");
        if (head->arg)
        {
            while (head->arg[i])
            {
                printf("'%s' ", head->arg[i]);
                i++;
            }
        }
        else
            printf("(none)");
        printf("\n");

        if (head->redirection && head->redir_count > 0)
{
    for (i = 0; i < head->redir_count; i++)
    {
        if (head->redirection[i])
        {
            printf("Redirection %d:\n", i);
            printf("  File: '%s'\n", head->redirection[i]->file ? head->redirection[i]->file : "(null)");
            printf("  Type: %d\n", head->redirection[i]->redir_type);
        }
        else
        {
            printf("Redirection %d is NULL\n", i);
        }
    }
}
else
{
    printf("Redirection: (none)\n");
}

        printf("Is pipe? %s\n", head->is_pipe ? "true" : "false");
        printf("------------------------\n");

        head = head->next;
        node_index++;
    }
}

