/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:05:12 by modat             #+#    #+#             */
/*   Updated: 2025/08/14 16:50:01 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to add argument to command
void	add_arg(char *token, t_command **current)
{
	int		i;
	char	**new_arg;

	i = 0;
	if ((*current)->arg)
		while ((*current)->arg[i])
			i++;
	new_arg = malloc(sizeof(char *) * (i + 2));
	if (!new_arg)
		return ;
	copy_existing_args(new_arg, (*current)->arg, i);
	new_arg[i] = ft_strdup(token);
	if (!new_arg[i])
	{
		free(new_arg);
		return ;
	}
	new_arg[i + 1] = NULL;
	if ((*current)->arg)
		free((*current)->arg);
	(*current)->arg = new_arg;
}

// Helper function to handle current command setup
int	handle_current_setup(t_command **current, t_command **cmd_list,
		char **new_tokens)
{
	*current = create_node();
	if (!*current)
	{
		free_tokens(new_tokens);
		if (*cmd_list)
			free_cmd(*cmd_list);
		return (0);
	}
	if (!*cmd_list)
		*cmd_list = *current;
	return (1);
}

void	handle_double_qoute(t_token_data *data)
{
	t_quote_data	quote_data;

	(void)data->wbeg;
	quote_data.quoted_len = parse_quoted_content(data->line, data->i,
			&quote_data.quoted_start);
	if (data->line[*data->i] == 34)
		(*data->i)++;
	quote_data.continuation_len = parse_continuation(data->line, data->i,
			&quote_data.continuation_start);
	build_double_quote_token(data->tokens, data->k, data->line, &quote_data);
}
