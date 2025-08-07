/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interative.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:30:15 by modat             #+#    #+#             */
/*   Updated: 2025/08/07 15:23:16 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 3
char	*append_char_to_line(char *line, int *line_len, char c)
{
	char	*new_line;

	new_line = (char *)realloc(line, *line_len + 2);
	if (!new_line)
	{
		free(line);
		return (NULL);
	}
	line = new_line;
	line[*line_len] = c;
	(*line_len)++;
	return (line);
}

// function - 4
static void	cleanup_read_buffer(t_read_buffer *rb)
{
	if (rb->buffer)
	{
		free(rb->buffer);
		rb->buffer = NULL;
	}
	rb->bytes_in_buffer = 0;
	rb->buffer_pos = 0;
}

// function - 5
static int	process_char(int fd, t_read_buffer *rb, char **line, int *line_len)
{
	int		read_status;
	char	temp_char;

	read_status = fill_buffer(fd, rb);
	if (read_status <= 0)
	{
		if (*line_len > 0 && read_status == 0)
		{
			(*line)[*line_len] = '\0';
			return (1);
		}
		return (-1);
	}
	temp_char = rb->buffer[rb->buffer_pos++];
	*line = append_char_to_line(*line, line_len, temp_char);
	if (!*line)
		return (-1);
	if (temp_char == '\n')
	{
		(*line)[*line_len - 1] = '\0';
		return (1);
	}
	return (0);
}

// function - 5
char	*readline_non_interactive(int fd)
{
	static t_read_buffer	rb = {NULL, 0, 0};
	char					*line;
	int						line_len;
	int						status;

	line = NULL;
	line_len = 0;
	if (!rb.buffer && init_read_buffer(&rb) == -1)
		return (NULL);
	while (1)
	{
		status = process_char(fd, &rb, &line, &line_len);
		if (status == 1)
		{
			cleanup_read_buffer(&rb);
			return (line);
		}
		else if (status == -1)
		{
			cleanup_read_buffer(&rb);
			return (NULL);
		}
	}
}
