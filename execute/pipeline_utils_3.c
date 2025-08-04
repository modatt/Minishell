/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 20:49:04 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 00:05:19 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
void	setup_output_pipe(t_command *current_cmd, int pipe_fds[2])
{
	if (current_cmd->next != NULL)
	{
		if (dup2(pipe_fds[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 stdout failed");
			exit(EXIT_FAILURE);
		}
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
}

// function - 2
int	create_pipe_and_fork(t_command *current_cmd, t_pipeline_data *data,
		int pipe_fds[2])
{
	pid_t	pid;

	if (current_cmd->next != NULL)
	{
		if (pipe(pipe_fds) == -1)
		{
			handle_pipe_error(*(data->prev_pipe_rd_fd), data->child_pids);
			return (-1);
		}
	}
	pid = fork();
	if (pid == -1)
	{
		handle_fork_error(current_cmd, pipe_fds, *(data->prev_pipe_rd_fd),
			data->child_pids);
		return (-1);
	}
	return (pid);
}

// function - 3
void	open_output_file(t_command *current_cmd, int i, int *fd_out)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (current_cmd->redirection[i]->redir_type == REDIR_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	*fd_out = open(current_cmd->redirection[i]->file, flags, 0644);
	if (*fd_out == -1)
	{
		perror(current_cmd->redirection[i]->file);
		exit(1);
	}
}

// function - 4
static void	handle_parent_process(t_command *current_cmd, pid_t pid,
		t_pipeline_data *data, int pipe_fds[2])
{
	data->child_pids[*(data->pid_count)] = pid;
	(*(data->pid_count))++;
	if (*(data->prev_pipe_rd_fd) != STDIN_FILENO)
		close(*(data->prev_pipe_rd_fd));
	if (current_cmd->next != NULL)
	{
		close(pipe_fds[1]);
		*(data->prev_pipe_rd_fd) = pipe_fds[0];
	}
	else
	{
		if (*(data->prev_pipe_rd_fd) != STDIN_FILENO)
			close(*(data->prev_pipe_rd_fd));
	}
}

// function - 5
int	setup_pipeline_execution(t_command *current_cmd, t_shell *shell,
		t_pipeline_data *data, int pipe_fds[2])
{
	pid_t	pid;

	pid = create_pipe_and_fork(current_cmd, data, pipe_fds);
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		free(data->child_pids);
		setup_input_redirection(*(data->prev_pipe_rd_fd));
		handle_input_file_redirection(current_cmd);
		setup_output_pipe(current_cmd, pipe_fds);
		handle_output_file_redirection(current_cmd);
		execute_command(current_cmd, shell);
	}
	else
		handle_parent_process(current_cmd, pid, data, pipe_fds);
	return (1);
}
