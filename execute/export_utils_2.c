/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:34:36 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 00:05:06 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
t_env_var	*create_var(char *name, char *value, bool booling)
{
	t_env_var	*new_var;

	if (!name)
		return (NULL);
	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
		return (NULL);
	if (init_var_fields(new_var, name, value, booling))
		return (new_var);
	free(new_var);
	return (NULL);
}

// function - 2
int	init_var_fields(t_env_var *var, char *name, char *value, bool exported)
{
	var->name = ft_strdup(name);
	if (!var->name)
	{
		free(var);
		return (0);
	}
	if (value)
	{
		var->value = ft_strdup(value);
		if (!var->value)
		{
			free(var->name);
			free(var);
			return (0);
		}
	}
	else
		var->value = NULL;
	var->exported = exported;
	var->next = NULL;
	return (1);
}

// function - 3
void	update_var_value(t_env_var *exist, char *new_value)
{
	if (!new_value)
		return ;
	free(exist->value);
	exist->value = ft_strdup(new_value);
}

// function - 4
void	free_name_value(char *name, char *value)
{
	free(name);
	free(value);
}

// function  - 5
void	name_invalid(char *name, char *value)
{
	if (value == NULL)
		print_export_error(name);
	else
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd("=", 2);
		ft_putstr_fd(value, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	(void)value;
}
