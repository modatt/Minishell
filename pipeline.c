/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 20:49:04 by modat             #+#    #+#             */
/*   Updated: 2025/07/26 12:45:40 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
static pid_t	*allocate_pid_array(int cmd_count)
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
static int	count_commands(t_command *cmd_list)
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
static void	handle_pipe_error(int prev_pipe_read_fd, pid_t *child_pids)
{
	perror("minishell: pipe failed");
	if (prev_pipe_read_fd != STDIN_FILENO)
	{
		close(prev_pipe_read_fd);
	}
	free(child_pids);
}

// function - 3

static void	handle_fork_error(t_command *current_cmd, int pipe_fds[2],
		int prev_pipe_read_fd, pid_t *child_pids)
{
	perror("minishell: fork failed");
	if (current_cmd->next != NULL)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	if (prev_pipe_read_fd != STDIN_FILENO)
	{
		close(prev_pipe_read_fd);
	}
	free(child_pids);
}

// function - 4
static void	setup_input_redirection(int prev_pipe_read_fd)
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

// function - 5
static void	handle_input_file_redirection(t_command *current_cmd)
{
	int	fd_in;

	for (int i = 0; i < current_cmd->redir_count; i++)
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
			break ;
		}
	}
}

// function - 6
static void	setup_output_pipe(t_command *current_cmd, int pipe_fds[2])
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

// function - 7
static void	handle_output_file_redirection(t_command *current_cmd)
{
	int	flags;
	int	fd_out;

	for (int i = 0; i < current_cmd->redir_count; i++)
	{
		if (current_cmd->redirection[i]->redir_type == REDIR_OUTPUT ||
			current_cmd->redirection[i]->redir_type == REDIR_APPEND)
		{
			flags = O_WRONLY | O_CREAT;
			if (current_cmd->redirection[i]->redir_type == REDIR_APPEND)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd_out = open(current_cmd->redirection[i]->file, flags, 0644);
			if (fd_out == -1)
			{
				perror(current_cmd->redirection[i]->file);
				exit(1);
			}
			if (dup2(fd_out, STDOUT_FILENO) == -1)
			{
				perror("minishell: dup2 output_file failed");
				close(fd_out);
				exit(EXIT_FAILURE);
			}
			close(fd_out);
			break ;
		}
	}
}

// function - 8
static void	execute_command(t_command *current_cmd, t_shell *shell)
{
	char	*cmd_path;

	if (is_builtin(current_cmd->arg[0]))
	{
		exec_builtin(current_cmd, shell);
		exit(shell->last_exit_status);
	}
	else
	{
		cmd_path = find_cmd_in_path(current_cmd->arg[0], shell);
		if (!cmd_path)
		{
			fprintf(stderr, "minishell: command not found: %s\n",
					current_cmd->arg[0]); // not allowed
			exit(127);
		}
		execve(cmd_path, current_cmd->arg, shell->envp);
		perror("minishell: execve failed");
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
}

// function - 9
static void	handle_child_process(t_command *current_cmd, t_shell *shell,
		int prev_pipe_read_fd, int pipe_fds[2], pid_t *child_pids)
{
	free(child_pids);
	setup_input_redirection(prev_pipe_read_fd);
	handle_input_file_redirection(current_cmd);
	setup_output_pipe(current_cmd, pipe_fds);
	handle_output_file_redirection(current_cmd);
	execute_command(current_cmd, shell);
}

// function - 10
static void	handle_parent_process(t_command *current_cmd, pid_t pid,
		pid_t *child_pids, int *pid_count, int *prev_pipe_read_fd,
		int pipe_fds[2])
{
	child_pids[*pid_count] = pid;
	(*pid_count)++;
	if (*prev_pipe_read_fd != STDIN_FILENO)
	{
		close(*prev_pipe_read_fd);
	}
	if (current_cmd->next != NULL)
	{
		close(pipe_fds[1]);
		*prev_pipe_read_fd = pipe_fds[0];
	}
	else
	{
		if (*prev_pipe_read_fd != STDIN_FILENO)
		{
			close(*prev_pipe_read_fd);
		}
	}
}
// function - 11
static void	wait_for_children(pid_t *child_pids, int pid_count, t_shell *shell)
{
	int	last_exit_status;
		int status;

	last_exit_status = 0;
	for (int i = 0; i < pid_count; i++)
	{
		if (waitpid(child_pids[i], &status, 0) == -1)
		{
			perror("minishell: waitpid failed");
			continue ;
		}
		if (i == pid_count - 1)
		{
			if (WIFEXITED(status))
			{
				last_exit_status = WEXITSTATUS(status);
			}
			else if (WIFSIGNALED(status))
			{
				last_exit_status = 128 + WTERMSIG(status);
			}
		}
	}
	shell->last_exit_status = last_exit_status;
}

// function - 12
void	execute_pipeline(t_command *cmd_list, t_shell *shell)
{
	int prev_pipe_read_fd = STDIN_FILENO;
	int pipe_fds[2];
	pid_t pid;
	pid_t *child_pids = NULL;
	int pid_count = 0;
	t_command *current_cmd = cmd_list;
	int cmd_count = count_commands(cmd_list);

	child_pids = allocate_pid_array(cmd_count);
	if (!child_pids)
		return ;

	while (current_cmd != NULL)
	{
		if (current_cmd->next != NULL)
		{
			if (pipe(pipe_fds) == -1)
			{
				handle_pipe_error(prev_pipe_read_fd, child_pids);
				return ;
			}
		}

		pid = fork();
		if (pid == -1)
		{
			handle_fork_error(current_cmd, pipe_fds, prev_pipe_read_fd,
					child_pids);
			return ;
        }
		if (pid == 0)
			handle_child_process(current_cmd, shell, prev_pipe_read_fd,
					pipe_fds, child_pids);
		else
			handle_parent_process(current_cmd, pid, child_pids, &pid_count,
					&prev_pipe_read_fd, pipe_fds);
		current_cmd = current_cmd->next;
	}

	wait_for_children(child_pids, pid_count, shell);
	free(child_pids);
}


/*
#include "minishell.h"

void execute_pipeline(t_command *cmd_list, t_shell *shell) 
{
    int     prev_pipe_read_fd = STDIN_FILENO;
    int     pipe_fds[2];
    pid_t   pid;
    pid_t   *child_pids = NULL;
    int     pid_count = 0;
    t_command *current_cmd = cmd_list;
    int cmd_count = 0;

    t_command *temp_cmd = cmd_list;
    while(temp_cmd) {
        cmd_count++;
        temp_cmd = temp_cmd->next;
    }

    child_pids = malloc(sizeof(pid_t) * cmd_count);
    if (!child_pids) {
        perror("minishell: malloc failed for child_pids");
        return;
    }

    int current_cmd_index = 0;

    while (current_cmd != NULL)
    {
        if (current_cmd->next != NULL) {
            if (pipe(pipe_fds) == -1) {
                perror("minishell: pipe failed");
                if (prev_pipe_read_fd != STDIN_FILENO) {
                    close(prev_pipe_read_fd);
                }
                free(child_pids);
                return;
            }
        }

        pid = fork();
        if (pid == -1) {
            perror("minishell: fork failed");
            if (current_cmd->next != NULL) {
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }
            if (prev_pipe_read_fd != STDIN_FILENO) {
                close(prev_pipe_read_fd);
            }
            free(child_pids);
            return;
        }

        if (pid == 0)
        {
            if (prev_pipe_read_fd != STDIN_FILENO) {
                if (dup2(prev_pipe_read_fd, STDIN_FILENO) == -1) {
                    perror("minishell: dup2 stdin failed");
                    exit(EXIT_FAILURE);
                }
                close(prev_pipe_read_fd);
            }
            int i = 0;
            
            while ( i < current_cmd->redir_count)
            {
                if (current_cmd->redirection[i]->redir_type == REDIR_INPUT) {
                    int fd_in = open(current_cmd->redirection[i]->file, O_RDONLY);
                    if (fd_in == -1) {
                        perror(current_cmd->redirection[i]->file);
                        exit(1);
                    }
                    if (dup2(fd_in, STDIN_FILENO) == -1) {
                        perror("minishell: dup2 input_file failed");
                        close(fd_in);
                        exit(EXIT_FAILURE);
                    }
                    close(fd_in);
                    break;
                }
                i++;
            }

            if (current_cmd->next != NULL) {
                if (dup2(pipe_fds[1], STDOUT_FILENO) == -1) {
                    perror("minishell: dup2 stdout failed");
                    exit(EXIT_FAILURE);
                }
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }
            
            int i = 0;
            while (i < current_cmd->redir_count) 
            {
                if (current_cmd->redirection[i]->redir_type == REDIR_OUTPUT ||
                    current_cmd->redirection[i]->redir_type == REDIR_APPEND) {
                    int flags = O_WRONLY | O_CREAT;
                    if (current_cmd->redirection[i]->redir_type == REDIR_APPEND) {
                        flags |= O_APPEND;
                    } else {
                        flags |= O_TRUNC;
                    }
                    int fd_out = open(current_cmd->redirection[i]->file, flags, 0644);
                    if (fd_out == -1) {
                        perror(current_cmd->redirection[i]->file);
                        exit(1);
                    }
                    if (dup2(fd_out, STDOUT_FILENO) == -1) {
                        perror("minishell: dup2 output_file failed");
                        close(fd_out);
                        exit(EXIT_FAILURE);
                    }
                    close(fd_out);
                    break;
                }
                i++;
            }

            if (is_builtin(current_cmd->arg[0])) {
                exec_builtin(current_cmd, shell);
                exit(shell->last_exit_status);
            } else {
                char *cmd_path = find_cmd_in_path(current_cmd->arg[0], shell);
                if (!cmd_path) {
                    fprintf(stderr, "minishell: command not found: %s\n", current_cmd->arg[0]);
                    exit(127);
                }
                execve(cmd_path, current_cmd->arg, shell->envp);
                perror("minishell: execve failed");
                free(cmd_path);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            child_pids[pid_count] = pid;
            pid_count++;

            if (prev_pipe_read_fd != STDIN_FILENO) {
                close(prev_pipe_read_fd);
            }
            if (current_cmd->next != NULL) {
                close(pipe_fds[1]);
                prev_pipe_read_fd = pipe_fds[0];
            } else {
                if (prev_pipe_read_fd != STDIN_FILENO) {
                    close(prev_pipe_read_fd);
                }
            }
        }
        current_cmd = current_cmd->next;
        current_cmd_index++;
    }

    int last_exit_status = 0;
    int i = 0;

    while (i < pid_count)
    {
        int status;
        if (waitpid(child_pids[i], &status, 0) == -1) {
            perror("minishell: waitpid failed");
            continue;
        }
        
        if (i == pid_count - 1) {
            if (WIFEXITED(status)) {
                last_exit_status = WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                last_exit_status = 128 + WTERMSIG(status);
            }
        }
        i++;
    }
    
    shell->last_exit_status = last_exit_status;
    free(child_pids);
}

*/
