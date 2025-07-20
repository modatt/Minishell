/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:27:08 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/07/20 17:56:59 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Reads data from fd into the buffer until \n or \0
static int	read_and_fill(int fd, char *buffer)
{
	char	*temp_str;
	int		bytes_read;

	bytes_read = 1;
	while (!(ft_strchr(buffer, '\n')) && bytes_read != 0)
	{
		temp_str = malloc(BUFFER_SIZE + 1);
		if (temp_str == NULL)
			return (-1);
		bytes_read = read(fd, temp_str, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(temp_str);
			return (-1);
		}
		temp_str[bytes_read] = '\0';
		ft_strlcat(buffer, temp_str, bytes_read);
		free(temp_str);
	}
	return (bytes_read);
}

//Extracts a line of specified length from the buffer.
static char	*extract_line(char *buffer, size_t length)
{
	char	*newline;

	newline = malloc(length + 1);
	if (newline == NULL)
	{
		return (NULL);
	}
	ft_strncpy(newline, buffer, length);
	newline[length] = '\0';
	return (newline);
}

static void	shift_buffer(char *buffer, size_t length)
{
	size_t	i;

	i = 0;
	while (i + length < ft_strlen(buffer))
	{
		buffer[i] = buffer[i + length];
		i++;
	}
	buffer[i] = '\0';
}

//reads a line from a file
char	*get_next_line(int fd)
{
	static char	buffer[100000];
	char		*line;
	size_t		length;

	if (read_and_fill(fd, buffer) == -1)
	{
		buffer[0] = '\0';
		return (NULL);
	}
	if (ft_strchr(buffer, '\n'))
	{
		length = ft_strchr(buffer, '\n') - buffer + 1;
		line = extract_line(buffer, length);
		shift_buffer(buffer, length);
		return (line);
	}
	if (buffer[0] != '\0')
	{
		line = extract_line(buffer, ft_strlen(buffer));
		buffer[0] = '\0';
		return (line);
	}
	return (NULL);

}
