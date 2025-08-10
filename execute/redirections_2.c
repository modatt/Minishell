/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:59:44 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/08/10 03:35:26 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// func------1
static int	open_heredoc_file(char *tmpfile)
{
	int	fd;

	fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell: heredoc open failed");
		free(tmpfile);
		return (-1);
	}
	return (fd);
}

// func------2
int	handle_heredoc_eof(char *line, int fd, char *tmpfile, char *delimiter)
{
	const char	*msg;

	(void)fd;
	(void)tmpfile;
	msg = "minishell: warning: here-document delimited by end-of-file "
		"(wanted `";
	close(fd);
	if (!line)
	{
		if (g_signal_status != 130)
		{
			write(STDERR_FILENO, msg, 69);
			write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
			write(STDERR_FILENO, "`)\n", 3);
		}
		return (1);
	}
	return (0);
}

// func --- 3
int	write_heredoc_to_file(char *tmpfile, char *delimiter)
{
	int		fd;
	char	*line;
	int		status;

	signals_heredoc();
	fd = open_heredoc_file(tmpfile);
	if (fd == -1)
		return (1);
	while (1)
	{
		line = readline("> ");
		if (g_signal_status == 130)
			return (heredoc_interrupted(line, fd));
		status = heredoc_process_line(line, delimiter, fd, tmpfile);
		if (status == 1)
			return (1);
		if (status == 2)
			break ;
	}
	close(fd);
	return (0);
}

// func --- 5
void	setup_redirection_fds(t_command *cmd)
{
	int		i;
	int		fd;
	t_redir	*redir;

	i = -1;
	while (++i < cmd->redir_count)
	{
		fd = -1;
		redir = cmd->redirection[i];
		fd = get_redirection_fd(redir);
		if (fd < 0)
			return ;
		if (redir->redir_type == REDIR_INPUT)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		if (fd != -1)
			close(fd);
	}
}
