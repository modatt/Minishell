/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:25 by modat             #+#    #+#             */
/*   Updated: 2025/07/29 17:34:30 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to copy existing arguments
void	copy_existing_args(char **new_arg, char **old_arg, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		new_arg[j] = old_arg[j];
		j++;
	}
}

// Helper function to initialize redirection array
int	init_redirection_array(t_command **current)
{
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
	return (1);
}

// Helper function to set redirection type
void	set_redir_type(t_redir *redir, char *token)
{
	if (ft_strcmp(token, ">") == 0)
		redir->redir_type = REDIR_OUTPUT;
	else if (ft_strcmp(token, ">>") == 0)
		redir->redir_type = REDIR_APPEND;
	else if (ft_strcmp(token, "<") == 0)
		redir->redir_type = REDIR_INPUT;
	else if (ft_strcmp(token, "<<") == 0)
		redir->redir_type = REDIR_HEREDOC;
}

// Helper function to process redirection file
int	process_redir_file(t_redir *redir, char **tokens, int *k)
{
	(*k)++;
	if (!tokens[*k])
	{
		free(redir);
		return (0);
	}
	redir->file = ft_strdup(tokens[*k]);
	if (!redir->file)
	{
		free(redir);
		return (0);
	}
	return (1);
}
