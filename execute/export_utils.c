/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:33:24 by modat             #+#    #+#             */
/*   Updated: 2025/08/07 14:11:32 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Export utils/ helper functions

// function - 1 // when passing export without args
// it will print the list of var

void	print_exported_list(t_shell *shell)
{
	t_env_var	*current;

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


static char *trim_value_part(char *str)
{
	int 	start;
	int 	end;
	int		len;

	start = 0;
	while (str[start] == ' ' || str[start] == '\t')
		start++;
	end = ft_strlen(str) - 1;
	while (end > start && (str[end] == ' ' || str[end] == '\t'))
		end--;
	len = end - start + 1;
	return (ft_substr(str, start, len));
}

void	parse_arg_var(char *arg, char **name, char **value)
{
	char	*is_equal_sign;
	int		equal_index;
	
	is_equal_sign = ft_strchr(arg, '=');
	if (is_equal_sign)
	{
		// equal_index++;
		equal_index = is_equal_sign - arg;
		(*name) = ft_substr(arg, 0, equal_index);
		(*value) = trim_value_part(arg + equal_index + 1);
	}
	else
	{
		(*name) = ft_strdup(arg);
		(*value) = NULL;
	}
}

// function - 3 // check if the name of the var is valid

bool	is_name_valid(const char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0])
		return (false);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (false);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

// function - 4 // print error in case of invalid var name
void	print_export_error(char *name)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
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
	// Don't free here - caller will handle cleanup
	(void)value; // Suppress unused parameter warning
}
