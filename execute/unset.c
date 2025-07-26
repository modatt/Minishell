#include "minishell.h"

int    builtin_unset(t_command *cmd, t_shell *shell)
{
    int i;

    i = 0;
       // If no arguments, do nothing
    if (!cmd || !cmd->arg || !cmd->arg[1])
        return -1;
    while (cmd->arg[i])
    {
        char *name = cmd->arg[i];

        // Check if name is a valid identifier
        if (is_name_valid(name) == true)
            remove_var(&shell->env_list, name);
        // else: silently ignore invalid names
        i++;
    }
    return 0;
}

void remove_var(t_env_var **env_list, const char *name)
{
    if (!env_list || !*env_list || !name)
        return ;
    t_env_var *current;
    t_env_var *prev;

    current = *env_list;
    prev = NULL;
    while (current)
    {
        if (strcmp(current->name, name) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env_list = current->next;

            free(current->name);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

