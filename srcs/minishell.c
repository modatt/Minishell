/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/06/02 17:28:21 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_cmd_list(t_command *head);

// Main Execution Loop

int main(void)
{
    greets_minishell();
    char *command_line;
    // t_command *list;
    char **tokens;
    int k = 0;
    
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

    // Processing the input through tokenizer
        // list = parser(command_line);
        tokens = new_parser(command_line);
        while (tokens[k])
        {
            printf("[%d] %s\n", k, tokens[k]);
            k++;
        }
        // print_cmd_list(list);


    // Executing commands through the flow control system
        // executor(list);


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

        // Print file and redirection
        if (head->file)
            printf("File: '%s'\n", head->file);
        else
            printf("File: (none)\n");

        printf("Redirection type: %d\n", head->redirection);
        printf("Is pipe? %s\n", head->is_pipe ? "true" : "false");
        printf("Contains wildcard? %s\n", head->is_wildcard ? "true" : "false");

        printf("------------------------\n");

        head = head->next;
        node_index++;
    }
}



