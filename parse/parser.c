/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:25 by modat             #+#    #+#             */
/*   Updated: 2025/07/26 16:57:00 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* parsing steps::
    1. tokenization: "enhancment"
        split string into words "edge case: qoutes"
        char **tokens;
    2. split commands: "need to be done"
        by operators
    3. add to data struct: 

*/


// static void print_tokens(char **tokens)
// {
//     int i = 0;
//     while (tokens[i] != NULL)
//     {
//         printf("Token[%d]: %s\n", i, tokens[i]);
//         i++;
//     }
// }



// main parsing function - 1
t_command    *parser(char *command_line, t_shell *shell)
{
    char **tokens;
    char **new_tokens;
    int k;
    t_command *cmd_list;
    t_command *current;

    k = 0;
    cmd_list = NULL;
    current =  NULL;
    tokens = tokenizer(command_line);
    if (!tokens)
        return NULL;
    // print_tokens(tokens);
    new_tokens = tokens_expanded(tokens, shell);
    if (!new_tokens)
    {
        free_tokens(tokens);  // Free original tokens if expansion fails
        return NULL;
    }
    while (new_tokens[k])
    {
        if (!current) // head 
        {
            current = create_node();
            if (!current)
            {
                free_tokens(new_tokens);
                if (cmd_list)
                    free_cmd(cmd_list);
                return NULL;
            }
            if (!cmd_list)
                cmd_list = current;
        }
        parser2(new_tokens, &k, &current);
        k++; 
    }
    free_tokens(new_tokens);  // Free the expanded tokens
    return cmd_list;
}



// function - 2

void    parser2(char **tokens, int *k, t_command **current)
{
    // connect and add based on conditions 
    if (ft_strcmp(tokens[*k], "|") == 0)
    {
        (*current)->is_pipe = true; 
        (*current)->next = create_node();
        if (!(*current)->next)
        {
            // Handle create_node failure - this is a serious error
            perror("Failed to create command node");
            return;
        }
        (*current) = (*current)->next;
    }
    else if (is_redirector(tokens, *k))
        is_redirection(tokens, current, k);
    else
        add_arg(tokens[*k], current);
    
}

// function - 3
void add_arg(char *token, t_command **current)
{
    int i = 0;
    int j = 0;
    char **new_arg;

    // Count current args
    if ((*current)->arg)
    {
        while ((*current)->arg[i])
            i++;
    }

    // Allocate space for existing args + 1 new arg + NULL terminator
    new_arg = malloc(sizeof(char *) * (i + 2));
    if (!new_arg)
        return; // malloc failure, just return (you may want to handle error differently)

    // Copy old pointers to new array
    while (j < i)
    {
        new_arg[j] = (*current)->arg[j];
        j++;
    }

    // Duplicate the new token string and add it
    new_arg[i] = ft_strdup(token);
    if (!new_arg[i])
    {
        // strdup failed, free allocated new_arg but not the strings (still pointed by old)
        free(new_arg);
        return;
    }

    // Null terminate the new array
    new_arg[i + 1] = NULL;

    // Free old array of pointers (not strings)
    if ((*current)->arg)
        free((*current)->arg);

    // Assign the new array to current command
    (*current)->arg = new_arg;
}


// function - 4
void is_redirection(char **tokens, t_command **current, int *k)
{
      if (!(*current)->redirection)
    {
        (*current)->redirection = malloc(sizeof(t_redir *) * 10); // Example: space for 10 redirs
        if (!(*current)->redirection)
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        (*current)->redir_count = 0;
    }
    t_redir *redir = malloc(sizeof(t_redir));
    if (!redir)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    if (ft_strcmp(tokens[*k], ">") == 0)
        redir->redir_type = REDIR_OUTPUT;
    else if (ft_strcmp(tokens[*k], ">>") == 0)
        redir->redir_type = REDIR_APPEND;
    else if (ft_strcmp(tokens[*k], "<") == 0)
        redir->redir_type = REDIR_INPUT;
    else if (ft_strcmp(tokens[*k], "<<") == 0)
        redir->redir_type = REDIR_HEREDOC;
    else
    {
        free(redir);
        return;
    }
    (*k)++; // Advance to file token
    if (!tokens[*k])  // Check if file token exists
    {
        free(redir);
        return;
    }
    redir->file = ft_strdup(tokens[*k]);
    if (!redir->file)  // Check if strdup failed
    {
        free(redir);
        return;
    }
    (*current)->redirection[(*current)->redir_count++] = redir;
}


// function - 5

