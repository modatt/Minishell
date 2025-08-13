/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:25 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 12:39:53 by modat            ###   ########.fr       */
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

// Helper function to initialize parser state
static int	init_parser_state(t_command **cmd_list, t_command **current)
{
	*cmd_list = NULL;
	*current = NULL;
	return (1);
}

// Helper function to expand tokens
static char	**expand_tokens_safe(char **tokens, t_shell *shell)
{
	char	**new_tokens;

	if (!tokens)
		return (NULL);
	new_tokens = tokens_expanded(tokens, shell);
	if (!new_tokens)
		free_tokens(tokens);
	return (new_tokens);
}

// main parsing function - 1

t_command	*parser_loop(char **new_tokens, t_command *cmd_list,
		t_command *current)
{
	int	k;

	k = 0;
	while (new_tokens[k])
	{
		if (!current)
			if (!handle_current_setup(&current, &cmd_list, new_tokens))
				return (NULL);
		parser2(new_tokens, &k, &current);
		k++;
	}
	return (cmd_list);
}

t_command	*parser(char *command_line, t_shell *shell)
{
	char		**tokens;
	char		**new_tokens;
	t_command	*cmd_list;
	t_command	*current;

	if (!command_line)
		return (NULL);
	tokens = tokenizer(command_line, shell);
	if (!tokens)
		return (NULL);
	init_parser_state(&cmd_list, &current);
	new_tokens = expand_tokens_safe(tokens, shell);
	if (!new_tokens)
		return (NULL);
	cmd_list = parser_loop(new_tokens, cmd_list, current);
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
