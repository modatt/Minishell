/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:59:32 by modat             #+#    #+#             */
/*   Updated: 2025/08/06 11:48:21 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// func --- 1
void	maybe_preprocess_heredocs(t_command *cmd)
{
	if (cmd && cmd->redir_count > 0)
		preprocess_heredocs(cmd);
}

// func --- 2
static int	process_single_heredoc(t_redir *redir, int index)
{
	char	*tmpfile;
	char	*idx_str;
	char	*delimiter;
	delimiter = ft_strdup(redir->file);
	if (!delimiter)
		return (1);
	idx_str = ft_itoa(index);
	tmpfile = ft_strjoin("/tmp/heredoc_", idx_str);
	free(idx_str);
	write_heredoc_to_file(tmpfile, delimiter);
	free(redir->file);
	free(delimiter);
	redir->file = tmpfile;
	redir->redir_type = REDIR_INPUT;
	return (0);
}

// func --- 3
void	preprocess_heredocs(t_command *cmd)
{
	int		i;
	t_redir	*redir;

	i = 0;
	while (i < cmd->redir_count)
	{
		redir = cmd->redirection[i];
		if (redir->redir_type == REDIR_HEREDOC)
		{
			if (process_single_heredoc(redir, i))
				return ;
		}
		i++;
	}
}
	