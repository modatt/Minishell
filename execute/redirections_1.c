/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:59:32 by modat             #+#    #+#             */
/*   Updated: 2025/08/08 11:48:10 by hmeltaha         ###   ########.fr       */
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
	int i; 

	if(redir->file == NULL)
	{
		write(1, "minishell: syntax error near unexpected token `newline'\n", 56);
		return(2);
	}
	delimiter = ft_strdup(redir->file);
	if (!delimiter)
		return (1);
	idx_str = ft_itoa(index);
	tmpfile = ft_strjoin("/tmp/heredoc_", idx_str);
	free(idx_str);
	i = write_heredoc_to_file(tmpfile, delimiter);
	free(redir->file);
	free(delimiter);
	redir->file = tmpfile;
	redir->redir_type = REDIR_INPUT;
	return (i);
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
			if (process_single_heredoc(redir, i) == 2)
				return ;
		}
		i++;
	}
}
//func --- 4	
void	run_with_redirection(t_command *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	saved_stderr;
	if (!shell->is_interactive)
		dup2(2, 0);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stderr = dup(STDERR_FILENO);
	if ((g_signal_status != 130) && (saved_stdin < 0 || saved_stdout < 0 || saved_stderr < 0))
	{
		perror("dup");
		return ;
	}
	if (g_signal_status != 130)
		setup_redirection_fds(cmd);
	
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	close(saved_stderr);
}
