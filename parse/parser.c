/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:25 by modat             #+#    #+#             */
/*   Updated: 2025/07/29 13:00:08 by modat            ###   ########.fr       */
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

// main parsing function - 1
t_command	*parser(char *command_line, t_shell *shell)
{
	char		**tokens;
	char		**new_tokens;
	int			k;
	t_command	*cmd_list;
	t_command	*current;

	k = 0;
	cmd_list = NULL;
	current = NULL;
	tokens = tokenizer(command_line);
	if (!tokens)
		return (NULL);
	new_tokens = tokens_expanded(tokens, shell);
	if (!new_tokens)
	{
		free_tokens(tokens);
		return (NULL);
	}
	while (new_tokens[k])
	{
		if (!current)
		{
			current = create_node();
			if (!current)
			{
				free_tokens(new_tokens);
				if (cmd_list)
					free_cmd(cmd_list);
				return (NULL);
			}
			if (!cmd_list)
				cmd_list = current;
		}
		parser2(new_tokens, &k, &current);
		k++;
	}
	free_tokens(new_tokens);
	return (cmd_list);
}

// function - 2

void	parser2(char **tokens, int *k, t_command **current)
{
	if (ft_strcmp(tokens[*k], "|") == 0)
	{
		(*current)->is_pipe = true;
		(*current)->next = create_node();
		if (!(*current)->next)
		{
			perror("Failed to create command node");
			return ;
		}
		(*current) = (*current)->next;
	}
	else if (is_redirector(tokens, *k))
		is_redirection(tokens, current, k);
	else
		add_arg(tokens[*k], current);
}

// function - 3
void	add_arg(char *token, t_command **current)
{
	int		i;
	int		j;
	char	**new_arg;

	i = 0;
	j = 0;
	if ((*current)->arg)
		while ((*current)->arg[i])
			i++;
	new_arg = malloc(sizeof(char *) * (i + 2));
	if (!new_arg)
		return ;
	while (j < i)
	{
		new_arg[j] = (*current)->arg[j];
		j++;
	}
	new_arg[i] = ft_strdup(token);
	if (!new_arg[i])
		return ;
	new_arg[i + 1] = NULL;
	if ((*current)->arg)
		free((*current)->arg);
	(*current)->arg = new_arg;
}

// function - 4
void	is_redirection(char **tokens, t_command **current, int *k)
{
	t_redir	*redir;

	if (!(*current)->redirection)
	{
		(*current)->redirection = malloc(sizeof(t_redir *) * 10);
		if (!(*current)->redirection)
		{
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}
		(*current)->redir_count = 0;
	}
	redir = malloc(sizeof(t_redir));
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
		return ;
	}
	(*k)++;
	if (!tokens[*k])
	{
		free(redir);
		return ;
	}
	redir->file = ft_strdup(tokens[*k]);
	if (!redir->file)
	{
		free(redir);
		return ;
	}
	(*current)->redirection[(*current)->redir_count++] = redir;
}

// function - 5
