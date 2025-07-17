#include "minishell.h"

void free_cmd(t_command *cmd)
{
    int i;
    t_command *tmp;

    while (cmd)
    {
        // Free arguments
        if (cmd->arg)
        {
            i = 0;
            while (cmd->arg[i])
            {
                free(cmd->arg[i]);
                i++;
            }
            free(cmd->arg);
        }

        // Free redirections
        free_redir(cmd);

        // Move to next and free current command
        tmp = cmd;
        cmd = cmd->next;
        free(tmp);
    }
}


void free_redir(t_command *cmd)
{
    int i = 0;

    if (cmd->redirection)
    {
        while (i < cmd->redir_count)
        {
            if (cmd->redirection[i])
            {
                free(cmd->redirection[i]->file);  // free the filename
                free(cmd->redirection[i]);        // free the redirection struct
            }
            i++;
        }
        free(cmd->redirection);  // free the array of pointers
    }
}


void    free_shell(t_shell *shell)
{
    //int i;
	//i = 0;
	// We did not allocate a memory for, we took it from the terminal
    /*if (shell->argv)
    {
        while (shell->argv[i])
        {
            free(shell->argv[i]);
			shell->argv[i] = NULL;
            i++;
        }
        free(shell->argv);
    }*/
	
    if (shell->envp)
	{
			//free_env_copy(shell->envp);
        int i = 0;
        while (shell->envp[i])
        {
            free(shell->envp[i]);
			shell->envp[i] = NULL;
            i++;
        }
        free(shell->envp);
		shell->envp = NULL;
	}
    free_env_list(shell);
    //free(shell);
}

void    free_env_list(t_shell *shell)
{
    t_env_var *tmp;

    while (shell->env_list)
    { 
        tmp = shell->env_list;
        shell->env_list = shell->env_list->next;
        free(shell->env_list->name);
        free(shell->env_list->value);
        free(tmp);
		if (shell->env_list->next == NULL)
			break ;
    }
}

