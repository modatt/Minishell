/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/06/25 17:57:17 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_cmd_list(t_command *head);


int main(int argc, char **argv, char **envp)
{
    greets_minishell();
    char *command_line;
    t_command *list;
    t_shell *shell;
    
    shell = malloc(sizeof(t_shell));
    if (!shell)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    init_shell(shell, argc, argv, envp);

    
    if (signals_handling() == -1)
        return -1;
        
    while (1)
    {
        // Setting up the shell environment
    // Reading user input with a custom prompt
    command_line = readline("minishell$ ");
    if (command_line == NULL)
        exit (-1);
    
    if (*command_line) //* if the first character not '\0' then the string isn't empty
        add_history(command_line);
    list = parser(command_line, shell);
    
    print_cmd_list(list);
    // Managing memory and cleaning up resources
        // free_list()
  
    }
    // free_cmd_list();
    // free_tokens();
    free(command_line);
    return 0;
}


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



// void print_cmd_list(t_command *head)
// {
//     int i;
//     int node_index = 0;

//     while (head)
//     {
//         printf("------ Command %d ------\n", node_index);

//         // Print arguments
//         i = 0;
//         printf("Arguments: ");
//         if (head->arg)
//         {
//             while (head->arg[i])
//             {
//                 printf("'%s' ", head->arg[i]);
//                 i++;
//             }
//         }
//         else
//             printf("(none)");
//         printf("\n");

//         // Print file and redirection
//         if (head->redirection->file)
//             printf("File: '%s'\n", head->redirection->file);
//         else
//             printf("File: (none)\n");

//         printf("Redirection type: %d\n", head->redirction);
//         printf("Is pipe? %s\n", head->is_pipe ? "true" : "false");
//         printf("Contains wildcard? %s\n", head->is_wildcard ? "true" : "false");

//         printf("------------------------\n");

//         head = head->next;
//         node_index++;
//     }
// }



