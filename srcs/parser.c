/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:25 by modat             #+#    #+#             */
/*   Updated: 2025/06/25 17:05:39 by modat            ###   ########.fr       */
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

t_command    *parser(char *command_line, t_shell *shell)
{
    // 1) tokenizing
    char **tokens;
    char **new_tokens;
    tokens = tokenizer(command_line);  
    if (!tokens)
        return NULL;
    new_tokens = tokens_expanded(tokens, shell);
    if (!new_tokens)
    {
        printf("new_tokens error\n");
        return NULL; 
    }
    // (void)shell;
    int k;
    t_command *cmd_list;
    t_command *current;

    k = 0;
    cmd_list = NULL;
    current =  NULL;
    while (new_tokens[k])
    {
        if (!current) // head 
        {
            current = create_node();
            if (!cmd_list)
                cmd_list = current;
        }
        parser2(new_tokens, &k, &current);
        k++; 
    }
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
    redir->file = ft_strdup(tokens[*k]);
    (*current)->redirection[(*current)->redir_count++] = redir;
}

/*
void    is_redirection(char **tokens, t_command **current, int *k)
{
    if (!(*current)->redirection)
    {
        (*current)->redirection = malloc(sizeof(t_redir *) * 1);
        if (!(*current)->redirection)
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        
    }
    t_redir *redir = malloc(sizeof(t_redir));
        if (!redir)
        {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
    if (ft_strcmp(tokens[*k], ">") == 0)
    {
        (*k)++;
        (*current)->redirection->file = ft_strdup(tokens[*k]);
        (*current)->redirection->redir_type = REDIR_OUTPUT;
    }
    else if (ft_strcmp(tokens[*k], ">>") == 0)
    {
        (*k)++;
        (*current)->redirection->file = ft_strdup(tokens[*k]);
        (*current)->redirection->redir_type = REDIR_APPEND;
    }
    else if (ft_strcmp(tokens[*k], "<") == 0)
    {
        (*k)++;
        (*current)->redirection->file = ft_strdup(tokens[*k]);
        (*current)->redirection->redir_type = REDIR_INPUT;
    }
    else if (ft_strcmp(tokens[*k], "<<") == 0)
    {
        (*k)++;
        (*current)->redirection->file = ft_strdup(tokens[*k]);
        (*current)->redirection->redir_type = REDIR_HEREDOC;
    }
    else 
    {
        free(redir);
        return ;
    }
}*/


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

