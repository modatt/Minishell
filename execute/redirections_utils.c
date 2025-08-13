/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 03:34:31 by hala              #+#    #+#             */
/*   Updated: 2025/08/10 03:34:34 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// func --- 1
static void	print_numric_error(char *arg)
{
	write(2, "minishell: ", 11);
	write(2, arg, ft_strlen(arg));
	write(2, ": No such file or directory\n", 28);
}

// func------2
int	get_redirection_fd(t_redir *redir)
{
	int	fd;

	if (redir->redir_type == REDIR_INPUT)
	{
		fd = open(redir->file, O_RDONLY);
		if (fd == -1)
			print_numric_error(redir->file);
	}
	else if (redir->redir_type == REDIR_OUTPUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

// func --- 3
int	heredoc_interrupted(char *line, int fd)
{
	signals_prompt();
	dup2(2, 0);
	if (line)
		free(line);
	close(fd);
	return (2);
}

// func --- 4
int	heredoc_process_line(char *line, char *delimiter, int fd, char *tmpfile)
{
	if (!line)
	{
		if (handle_heredoc_eof(line, fd, tmpfile, delimiter))
			return (1);
		return (0);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (2);
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (0);
}
