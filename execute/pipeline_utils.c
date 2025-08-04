/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 20:49:04 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 00:05:51 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
pid_t	*allocate_pid_array(int cmd_count)
{
	pid_t	*child_pids;

	child_pids = malloc(sizeof(pid_t) * cmd_count);
	if (!child_pids)
	{
		perror("minishell: malloc failed for child_pids");
		return (NULL);
	}
	return (child_pids);
}

// function - 2
int	count_commands(t_command *cmd_list)
{
	int			cmd_count;
	t_command	*temp_cmd;

	cmd_count = 0;
	temp_cmd = cmd_list;
	while (temp_cmd)
	{
		cmd_count++;
		temp_cmd = temp_cmd->next;
	}
	return (cmd_count);
}

// function - 3
void	handle_pipe_error(int prev_pipe_read_fd, pid_t *child_pids)
{
	perror("minishell: pipe failed");
	if (prev_pipe_read_fd != STDIN_FILENO)
		close(prev_pipe_read_fd);
	free(child_pids);
}

// function - 4
void	handle_fork_error(t_command *current_cmd, int pipe_fds[2],
		int prev_pipe_read_fd, pid_t *child_pids)
{
	perror("minishell: fork failed");
	if (current_cmd->next != NULL)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	if (prev_pipe_read_fd != STDIN_FILENO)
		close(prev_pipe_read_fd);
	free(child_pids);
}

// function - 5
void	setup_input_redirection(int prev_pipe_read_fd)
{
	if (prev_pipe_read_fd != STDIN_FILENO)
	{
		if (dup2(prev_pipe_read_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 stdin failed");
			exit(EXIT_FAILURE);
		}
		close(prev_pipe_read_fd);
	}
}
