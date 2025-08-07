/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:59:44 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/08/07 11:16:21 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//func------1
static int	open_heredoc_file(char *tmpfile)
{
	int	fd;

	fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell: heredoc open failed");
		free(tmpfile);
		return(-1);
	}
	return (fd);
}

//func------2
static int	handle_heredoc_eof(char *line, int fd, char *tmpfile,char *delimiter)
{
	(void)fd;
	(void)tmpfile;
	const char	*msg;

	msg = "minishell: warning: here-document delimited by end-of-file "
		"(wanted `";
	// heredoc interruption is now tracked in shell struct, not global
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

	signals_heredoc();
	fd = open_heredoc_file(tmpfile);
	if (fd == -1)
		return(1);
	while (1)
	{
		line = readline("> ");
		//fprintf(stderr, "--->%d \n", g_signal_status);
		if (g_signal_status == 130)
		{
			signals_prompt();
			dup2(2, 0);
			if (line)
				free(line);
			return(2);
		}
		if (!line)
		{
			if (handle_heredoc_eof(line, fd, tmpfile, delimiter))
				return(1);
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
	return(0);
}

//func------4
static void	print_numric_error(char *arg)
{
	write(2, "minishell: ", 11);
	write(2, arg, ft_strlen(arg));
	write(2, ": No such file or directory\n", 28);
}

// func --- 5
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
		{
			fd = open(redir->file, O_RDONLY);
			if(fd == -1)
				print_numric_error(redir->file);
		}
		else if (redir->redir_type == REDIR_OUTPUT)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->redir_type == REDIR_APPEND)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			return ;
		if (redir->redir_type == REDIR_INPUT)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		if(fd != -1)
			close(fd);
		i++;
	}
}
