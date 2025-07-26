#include <unistd.h>
#include "minishell"


// count cmd or pipes 
int     pipe_count(t_command *cmd)
{
    int pipe; 
    t_command current_cmd;

    current_cmd = cmd;
    while (current_cmd)
    {
        if (current_cmd->is_pipe)
            pipe++;
        current_cmd = current_cmd->next;
    }
    return pipe;
}



void    execute_pipe(t_command *cmd)
{
    int     pipe_nbr;
    int     pipefd[2];
    int     prev_pipe[2] = {-1, -1};
    pid_t   *children_pid;
    t_command *current;

    current = cmd;
    pipe_nbr = pipe_count(current);
    children_pid = allocate_child_pid_array(pipe_nbr);
    while (current)
    {
        pipe_per_cmd(current);
        pid = fork();  
        handle_fork_error(pid);
        if (pid == 0) // child process
            handle_child_process(prev_pipe, pipefd, current, shell);
        else // parent process
            int pid_index = handle_parent_process(children_pid, pid)
        current = current->next;
    }
     // Step 6: Wait for all children
    wait_chidren(current, shell, pid_index);
     free(children_pid);
}






















void    execute_pipe(t_command *cmd)
{
    // loop each cmd
    // cmd list && tem_list store in it to loop
    t_command current_list;
    int pipe_nbr;
    int     pipefd[2];
    
    cmd_list = cmd;
    pipe_nbr = pipe_count(current_list);

    while (current_list)
    {
        //1) create pipe for each cmd
        // need an array int array
        // the array needs a size, the size is 2 for each pipe
       // create a pipe for each cmd
        if (current_cmd->is_pipe)
        {
            if (pipe(pipefd2) == -1) // create 2nd pipe in parent then first pipe in child to be run first
            {
                perror("pipe");
                return 1;
            }
        }

        // manage pipe fds pipefd[] array for more than 2 pipes to not overwrite
        // do 2nd pipe then first 
        // now fork() process the child & create the first pipe.

        // manage pipe fds pipefd[] array for more than 2 pipes to not overwrite
        // You create pipes in the parent BEFORE you fork children.


        // Now I have 2 fds : pipefdd[0] == read end "here the child or second process read the output of the first process" && pipefd[1] == write end
        // pipe is for the communication between 2 processes therefore it comes with fork()
        // create processes for communication
          pid = fork();  // returns 0 for child & pid of child process to parent to control it & manage it 
        // now I have 2 processes to do what I want to do. 

        if (pid == -1)
        {
            perror("fork");
            return 1;
        }
        if (pipe == 0)
            close(pipefd[1]); // close write end if only reading
            // write the result of the command on write end fd
        else 
            close(pipefd[0]); // close read end in parent if only writing
            // you can write to pipefd[1];
            // read end fd & write on write end keep it to next cmd

        // now you can use read() & write() to read and write exchange data through pipe
    
    current_list = current_list->next;
    }

}