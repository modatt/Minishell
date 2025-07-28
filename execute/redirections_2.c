/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:59:44 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/07/28 14:25:05 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_heredoc_file(char *tmpfile)
{
	int	fd;

	fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("minishell: heredoc open failed");
		free(tmpfile);
		signals_prompt();
		exit(1);
	}
	return (fd);
}

static int	handle_heredoc_eof(char *line, int fd, char *tmpfile,
		char *delimiter)
{
	const char	*msg;

	msg = "minishell: warning: here-document delimited by end-of-file "
		"(wanted `";
	if (g_heredoc_interrupted)
	{
		close(fd);
		unlink(tmpfile);
		g_heredoc_interrupted = 130;
		signals_prompt();
		return (1);
	}
	else if (!line && g_heredoc_interrupted != 130)
	{
		write(STDERR_FILENO, msg, 69);
		write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
		write(STDERR_FILENO, "`)\n", 3);
		return (1);
	}
	return (0);
}

// func --- 3
void	write_heredoc_to_file(char *tmpfile, char *delimiter)
{
	int		fd;
	char	*line;

	rl_catch_signals = 0;
	signals_heredoc();
	fd = open_heredoc_file(tmpfile);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (handle_heredoc_eof(line, fd, tmpfile, delimiter))
				return ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	signals_prompt();
}

// func --- 1
void	setup_redirection_fds(t_command *cmd)
{
	int		i;
	int		fd;
	t_redir	*redir;

	i = 0;
	while (i < cmd->redir_count)
	{
		fd = -1;
		redir = cmd->redirection[i];
		if (redir->redir_type == REDIR_INPUT)
			fd = open(redir->file, O_RDONLY);
		else if (redir->redir_type == REDIR_OUTPUT)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->redir_type == REDIR_APPEND)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			exit(1);
		if (redir->redir_type == REDIR_INPUT)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		i++;
	}
}
