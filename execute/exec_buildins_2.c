#include "minishell.h"


// function --- 1
static int is_n_flag(const char *arg)
{
    int i;

    if (!arg || arg[0] != '-')  // must start with '-'
        return (0);
    i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n')     // if anything other than 'n' → NOT a flag
            return (0);
        i++;
    }
    return (1); // valid -n flag (like -n, -nn, -nnnnn)
}

// function --- 2
void builtin_echo(t_command *cmd)
{
    int i;
    int no_newline;

	no_newline = 0;
    if (!cmd->arg[1])
    {
        printf("\n");
        return;
    }
    i = 1;
    while (cmd->arg[i] && is_n_flag(cmd->arg[i]))
    {
        no_newline = 1;  // means at least one valid -n found
        i++;
    }
    //Print remaining arguments
    while (cmd->arg[i])
    {
        printf("%s", cmd->arg[i]);
        i++;
        if (cmd->arg[i])
            printf(" ");
    }
    if (!no_newline)
        printf("\n");
}



// function ---3
void	builtin_pwd(t_command *cmd)
{
	char	path[PATH_MAX + 1];

	(void)cmd;
	if (getcwd(path, sizeof(path)))
		printf("%s \n", path);
	return ;
}

// function ---4
void 	builtin_env(t_shell *shell)
{
	t_env_var *current;

	current = shell->env_list;
	while(current)
	{
		if (current->value)
			printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}

// function ---5
int     handle_export_var_cd(char *name, char *value, t_shell *shell, int status)
{
	if (name == NULL ||  name[0] == '\0' || !is_name_valid(name))
    {
		name_invalid(name, value);
        status = 1; // faild
    }
    else 
    { // no allocation because this is for search and find if var already exist
        t_env_var *exist = find_var(shell->env_list, name);
        if (exist)
        {
			if (value)
			update_var_value(exist, value);
            exist->exported = true;
        }
        else 
        {
			t_env_var *new_var = create_var(name, value, true);
            if (new_var)
			add_var_to_list(&shell->env_list, new_var);
            else
			status = 1;
        }
    }
    return status;
}


// // function - 5


