#include "minishell.h"

// Export utils/ helper functions

// function - 1 // when passing export without args 
// it will print the list of var

void    print_exported_list(t_shell *shell)
{
    t_env_var *current;
    current = shell->env_list;
    while (current)
    {
        if (current->exported)
        {
            if (current->value)
                printf("declare -x %s=\"%s\"\n", current->name, current->value);
            else
                printf("declare -x %s\n", current->name);
        }
        current = current->next;
    }
}

// function - 2 // in case of export with args
// parsing args and split into name & value

void    parse_arg_var(char *arg, char **name, char **value)
{
    char *is_equal_sign = ft_strchr(arg, '=');
    if (is_equal_sign)
    {
        // split arg into name & value at =.
        int equal_index = is_equal_sign - arg;
        (*name) = ft_substr(arg, 0, equal_index);
        (*value) = ft_strdup(arg + equal_index + 1);
    }
    else
    {
        (*name) = ft_strdup(arg);
        (*value) = NULL;
    }
}

// function - 3 // check if the name of the var is valid

bool    is_name_valid(const char *name)
{
    int i = 0;

    if (!name || !name[0])
        return false;
    // check first char 
    if (!ft_isalpha(name[0]) && name[0] != '_')
        return false;
    // check the rest of chara 
    i = 1; 
    while (name[i])
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return false;
        i++;
    }
    return true; 
}

// function - 4 // print error in case of invalid var name
void    print_export_error(char *name)
{
    // putstr and 2 to send it to strerr 
    ft_putstr_fd("minishell: export: `", 2);
    ft_putstr_fd(name, 2);
    ft_putstr_fd("': not a valid identifier\n", 2);
}

// function  - 5
void    name_invalid(char *name, char *value)
{
    print_export_error(name);
    free(name);
    free(value);
    // return ;
}

