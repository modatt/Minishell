/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 12:38:15 by modat             #+#    #+#             */
/*   Updated: 2025/07/26 12:42:21 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t   *allocate_child_pid_array(int pipe_nbr)
{
    pid_t   *child_pid;

    child_pid = malloc(sizeof(pid_t) * pipe_nbr + 1);
    if (!child_pid)
    {
        perror("minishell: malloc failed for child_pids");
        exit(EXIT_FAILURE);
    }
    return (child_pid);
}


void handle_fork_error(pid_t     pid)
{
    if (pid == -1)
        {
            perror("minishell: fork faild");
            exit(EXIT_FAILURE);
        }
}    

void    handle_child_process(int *prev_pipe, int *pipefd, t_command *cmd, t_shell *shell)
{
    if (prev_pipe[0] != -1)
    {
          // Step 3.1: Setup input (prev pipe or file)
            // Step 3.2: Setup output (next pipe or file)
            // Step 3.3: Close unused FDs
            // Step 3.4: Execute command (execve or built-in)
            // Step 3.5: exit
        dup2(prev_pipe[0], STDIN_FILENO);
        close(prev_pipe[0]);
        close(prev_pipe[1]);
    }
    if (cmd->is_pipe)
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
    }
    setup_redirection_fds(cmd);
    execute_cmd(cmd, shell);
    exit(127); // command not found not sure about it 

}

int    handle_parent_process(pid_t  *child_pid, pid_t pid, int *prev_pipe, t_command *cmd, int *pipefd)
{
    int     pid_index;

    pid_index = 0;
    // Step 4.1: Store child's PID
    child_pid[pid_index++] = pid;
    // Step 4.2: Close prev pipe
    if (prev_pipe[0] != -1)
    {
        close(prev_pipe[0]);
        close(prev_pipe[1]);
    }
     // Step 4.3: Update prev_pipe to current pipe
    if (cmd->is_pipe)
    {
        prev_pipe[0] = pipefd[0];
        prev_pipe[1] = pipefd[1];
    }
    return pid_index;
}

void    pipe_per_cmd(t_command *cmd, int *pipefd)
{
      if (cmd->is_pipe)
        {
            if (pipe(pipefd) == -1) 
            {
                perror("minishell: pipe faild");
                exit(EXIT_FAILURE);
            }
        }
}

void    wait_children(t_command *cmd, t_shell *shell, int pid_index)
{
    int last_exit_status = 0;
    int i = 0;

    while (i < pid_index)
    {
        int status;
        if (waitpid(child_pids[i], &status, 0) == -1) {
            perror("minishell: waitpid failed");
            continue;
        }      
        if (i ==  pipe_index)
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
        i++;
    }
    
    shell->last_exit_status = last_exit_status;
}
