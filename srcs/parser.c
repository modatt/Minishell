/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:25 by modat             #+#    #+#             */
/*   Updated: 2025/06/18 08:18:53 by modat            ###   ########.fr       */
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


// main parsing function 

t_command    *parser(char *command_line)
{
    // 1) tokenizing
    char **tokens;
    tokens = tokenizer(command_line);  
    if (!tokens)
        return NULL;
    // 2) parse tokens & create a list & fill it
    int k;
    t_command *cmd_list;
    t_command *current;

    k = 0;
    cmd_list = NULL;
    current =  NULL;
    // (void)cmd_list;

    while (tokens[k])
    {
        if (!current) // head 
        {
            current = create_node();
            if (!cmd_list)
                cmd_list = current;
        }
        parser2(tokens, &k, &current);
        k++; 
    }
    // cmd_list = current;
    return cmd_list;
}


void    parser2(char **tokens, int *k, t_command **current)
{
    // connect and add based on conditions 
    if (ft_strcmp(tokens[*k], "|") == 0)
    {
        (*current)->is_pipe = true; 
        (*current)->next = create_node();
        (*current) = (*current)->next;
    }
    else if (is_redirector(tokens, *k))
        is_redirection(tokens, current, k);
    else
    {
        add_arg(tokens[*k], current);
        if (expand_wildcard(tokens[*k]) == true)
            (*current)->is_wildcard = true;
    }
    
}


void    add_arg(char *tokens, t_command **current)
{
    int i;
    int j;
    char **new_arg;
    // count current args
    i = 0;
    j = 0;
    if ((*current)->arg)
        while ((*current)->arg[i])
            i++;
    // Allocate space for one new arg + NULL terminator
    new_arg = malloc(sizeof(char *) * (i + 2));
    if (!new_arg)
        return ;
    // Copy old args
    while (j < i)
    {
        new_arg[j] = (*current)->arg[j];
        j++;
    }
    // Add new arg
    new_arg[i] = ft_strdup(tokens);
    new_arg[i + 1] = NULL;

    // Free old array if it exists (not the strings inside, just the array)
    if ((*current)->arg)
        free((*current)->arg);       
    // Assign new array to cmd
    (*current)->arg = new_arg;  
}


bool is_redirector(char **tokens, int k)
{
    return (
        ft_strcmp(tokens[k], ">>") == 0 ||
        ft_strcmp(tokens[k], ">") == 0 ||
        ft_strcmp(tokens[k], "<<") == 0 ||
        ft_strcmp(tokens[k], "<") == 0
    );
}

void    is_redirection(char **tokens, t_command **current, int *k)
{
    (void)current;
    if (ft_strcmp(tokens[*k], ">") == 0)
    {
        (*k)++;
        // (*current)->file = ft_strdup(tokens[*k]);
        // (*current)->redirection = REDIR_OUTPUT;
    }
    else if (ft_strcmp(tokens[*k], ">>") == 0)
    {
        (*k)++;
        // (*current)->file = ft_strdup(tokens[*k]);
        // (*current)->redirection = REDIR_APPEND;
    }
    else if (ft_strcmp(tokens[*k], "<") == 0)
    {
        (*k)++;
        // (*current)->file = ft_strdup(tokens[*k]);
        // (*current)->redirection = REDIR_INPUT;
    }
    else if (ft_strcmp(tokens[*k], "<<") == 0)
    {
        (*k)++;
        // (*current)->file = ft_strdup(tokens[*k]);
        // (*current)->redirection = REDIR_HEREDOC;
    }
}


bool    expand_wildcard(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '*')
            return true;
        i++;
    }
    return false;
}


t_command   *create_node()
{
    t_command *new_node;

    new_node = malloc(sizeof(t_command));
    if (!new_node)
        return NULL;

    init_cmd(new_node);
    return (new_node);
}

