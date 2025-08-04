/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interative.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:30:15 by modat             #+#    #+#             */
/*   Updated: 2025/07/31 15:09:54 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1

// read_line_non_interactive.c (or you can put this in a utils.c or main_loop.c)

// Initializes the read buffer structure.
// Returns 0 on success, -1 on allocation failure.
static int init_read_buffer(t_read_buffer *rb)
{
    rb->buffer = (char *)malloc(BUFFER_SIZE + 1);
    if (!rb->buffer)
        return (-1); // Allocation failed
    rb->bytes_in_buffer = 0;
    rb->buffer_pos = 0;
    return (0); // Success
}

// Reads more data into the given buffer if it's exhausted.
// Returns the number of bytes read, 0 on EOF, or -1 on error.
static int fill_buffer(int fd, t_read_buffer *rb)
{
    if (rb->buffer_pos >= rb->bytes_in_buffer)
    {
        int read_bytes = read(fd, rb->buffer, BUFFER_SIZE);
        if (read_bytes <= 0) // EOF or error
        {
            return (read_bytes); // 0 for EOF, -1 for error
        }
        rb->bytes_in_buffer = read_bytes;
        rb->buffer_pos = 0;
    }
    return (rb->bytes_in_buffer); // Return bytes currently available in buffer
}

// Appends a character to the line, reallocating if necessary.
// Returns the new line pointer, or NULL on allocation failure.
static char *append_char_to_line(char *line, int *line_len, char c)
{
    char *new_line = (char *)realloc(line, *line_len + 2); // +1 for char, +1 for null terminator
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

// Cleans up the buffer when the reading is complete or on error.
static void cleanup_read_buffer(t_read_buffer *rb)
{
    if (rb->buffer)
    {
        free(rb->buffer);
        rb->buffer = NULL;
    }
    rb->bytes_in_buffer = 0;
    rb->buffer_pos = 0;
}

// Handles the logic for reading a single character and appending it to the line.
// Returns 1 if a newline is found, 0 if more reading is needed, -1 on error/EOF.
static int process_char(int fd, t_read_buffer *rb, char **line, int *line_len)
{
    int read_status;
    char temp_char;

    read_status = fill_buffer(fd, rb);
    if (read_status <= 0) // EOF or error
    {
        if (*line_len > 0 && read_status == 0) // Return accumulated line on EOF
        {
            (*line)[*line_len] = '\0';
            return (1); // Indicate line is complete (EOF)
        }
        return (-1); // Indicate error or no line to return
    }
    temp_char = rb->buffer[rb->buffer_pos++];
    *line = append_char_to_line(*line, line_len, temp_char);
    if (!*line)
        return (-1); // Allocation failure

    if (temp_char == '\n')
    {
        (*line)[*line_len - 1] = '\0'; // Replace newline with null terminator
        return (1); // Indicate newline found, line is complete
    }
    return (0); // Indicate more reading is needed
}

// This function reads a line from the given file descriptor (fd)
// It handles dynamic memory allocation and returns NULL on EOF or error.
char *readline_non_interactive(int fd)
{
    static t_read_buffer rb = {NULL, 0, 0}; // Static instance of the buffer state
    char *line = NULL;
    int line_len = 0;
    int status;

    // Initialize buffer if it's the first call or was freed
    if (!rb.buffer && init_read_buffer(&rb) == -1)
        return (NULL);

    while (1)
    {
        status = process_char(fd, &rb, &line, &line_len);
        if (status == 1) // Line is complete (newline or EOF with data)
        {
            cleanup_read_buffer(&rb); // Free buffer on final EOF or newline
            return (line);
        }
        else if (status == -1) // Error or EOF with no data
        {
            cleanup_read_buffer(&rb); // Free buffer on error or EOF with no line
            return (NULL);
        }
        // If status is 0, continue the loop for more characters
    }
}
