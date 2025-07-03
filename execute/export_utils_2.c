#include "minishell.h"



// function - 1
t_env_var *create_var(char *name, char *value, bool booling)
{
    t_env_var *new_var;

    if (!name)
        return NULL;
    new_var = malloc(sizeof(t_env_var));
    if (!new_var)
        return NULL;
    if (init_var_fields(new_var, name, value, booling))
        return new_var;
    return NULL;
}

// function - 2

int init_var_fields(t_env_var *var, char *name, char *value, bool exported)
{
    var->name = ft_strdup(name);
    if (!var->name)
    {
        free(var);
        return 0;
    }
    if (value)
    {
        var->value = ft_strdup(value);
        if (!var->value)
        {
            free(var->name);
            free(var);
            return 0;
        }
    }
    else
        var->value = NULL;
    var->exported = exported;
    var->next = NULL;
    return 1;
}

// function - 3

void    update_var_value(t_env_var *exist, char *new_value)
{
    if (!new_value)
        return ;
    // free old value
    free(exist->value);
    // in case of new_value is null >> store empty string or null
    // if (new_value)
        exist->value = ft_strdup(new_value);
    // else
    //     exist->value = NULL; // empty case 
}


// function - 4
void    free_name_value(char *name, char *value)
{
    free(name);
    free(value);
}
