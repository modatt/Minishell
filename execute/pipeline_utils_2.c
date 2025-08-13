/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 20:49:04 by modat             #+#    #+#             */
/*   Updated: 2025/08/11 11:49:41 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
void	handle_output_file_redi(t_command *current_cmd)
{
	int	fd_out;
	int	i;

	i = 0;
	while (i < current_cmd->redir_count)
	{
		if (current_cmd->redirection[i]->redir_type == REDIR_OUTPUT
			|| current_cmd->redirection[i]->redir_type == REDIR_APPEND)
		{
			open_output_file(current_cmd, i, &fd_out);
			if (dup2(fd_out, STDOUT_FILENO) == -1)
			{
				perror("minishell: dup2 output_file failed");
				close(fd_out);
				exit(EXIT_FAILURE);
			}
			close(fd_out);
		}
		i++;
	}
}

// function - 2
void	handle_input_file_redir(t_command *current_cmd)
{
	int	fd_in;
	int	i;

	i = 0;
	while (i < current_cmd->redir_count)
	{
		if (current_cmd->redirection[i]->redir_type == REDIR_INPUT)
		{
			fd_in = open(current_cmd->redirection[i]->file, O_RDONLY);
			if (fd_in == -1)
			{
				perror(current_cmd->redirection[i]->file);
				exit(1);
			}
			if (dup2(fd_in, STDIN_FILENO) == -1)
			{
				perror("minishell: dup2 input_file failed");
				close(fd_in);
				exit(EXIT_FAILURE);
			}
			close(fd_in);
		}
		i++;
	}
}

// function - 3
void	wait_for_children(int pid_count, t_shell *shell)
{
	int	status;
	int	i;

	i = 0;
	while (i < pid_count)
	{
		handle_signal_pipe();
		if (waitpid(-1, &status, 0) == -1)
		{
			perror("minishell: waitpid failed");
			i++;
			continue ;
		}
		if (i == pid_count - 1)
		{
			if (WIFEXITED(status))
				shell->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->last_exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	signals_prompt();
}

// function - 4
void	execute_command(t_command *current_cmd, t_shell *shell)
{
	char	*cmd_path;

	if (!current_cmd->arg || !current_cmd->arg[0])
		clean_exit(shell, shell->last_exit_status, current_cmd);
	if (is_builtin(current_cmd->arg[0]))
	{
		exec_builtin(current_cmd, shell);
		clean_exit(shell, shell->last_exit_status, current_cmd);
	}
	else
	{
		cmd_path = find_cmd_in_path(current_cmd->arg[0], shell);
		if (!cmd_path)
		{
			write(2, "minishell: command not found: ", 30);
			write(2, current_cmd->arg[0], ft_strlen(current_cmd->arg[0]));
			write(2, "\n", 1);
			exit(127);
		}
		execve(cmd_path, current_cmd->arg, shell->envp);
		perror("minishell: execve failed");
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
}

// function - 5
void	init_pipeline_data(t_command *cmd_list, t_pipeline_data *data,
		int *prev_pipe_rd_fd, int *pid_count)
{
	*prev_pipe_rd_fd = STDIN_FILENO;
	*pid_count = 0;
	data->child_pids = allocate_pid_array(count_commands(cmd_list));
	data->pid_count = pid_count;
	data->prev_pipe_rd_fd = prev_pipe_rd_fd;
}
