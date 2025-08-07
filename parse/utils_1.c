/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:09:51 by modat             #+#    #+#             */
/*   Updated: 2025/08/07 15:23:53 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// func - 1
void	handler_heredoc(int sig)
{
	write(1, "\n", 1);
	close(0);
	if (sig == SIGINT)
		g_signal_status = 130;
}

// func - 2
void	handle_input_status(t_shell *shell, int status, char *cmd)
{
	if (status == INPUT_OK)
		process_command(shell, cmd);
}

// func - 3
t_shell	*create_shell(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("Failed to allocate shell structure");
		return (NULL);
	}
	if ((init_shell(shell, argc, argv, envp) != 0))
	{
		printf("Failed to initialize shell\n");
		free(shell);
		return (NULL);
	}
	if (!shell->envp)
		printf("Warning: Failed to initialize environment\n");
	return (shell);
}

// function - 4
int	init_read_buffer(t_read_buffer *rb)
{
	rb->buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!rb->buffer)
		return (-1);
	rb->bytes_in_buffer = 0;
	rb->buffer_pos = 0;
	return (0);
}

// function - 5
int	fill_buffer(int fd, t_read_buffer *rb)
{
	int	read_bytes;

	if (rb->buffer_pos >= rb->bytes_in_buffer)
	{
		read_bytes = read(fd, rb->buffer, BUFFER_SIZE);
		if (read_bytes <= 0)
		{
			return (read_bytes);
		}
		rb->bytes_in_buffer = read_bytes;
		rb->buffer_pos = 0;
	}
	return (rb->bytes_in_buffer);
}
