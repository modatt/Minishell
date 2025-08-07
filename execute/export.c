/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:32:19 by modat             #+#    #+#             */
/*   Updated: 2025/08/07 14:12:35 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(t_command *cmd, t_shell *shell)
{
	int		status;
	int		i;
	char	*name;
	char	*value;

	status = 0;
	if (cmd->arg[1] == NULL)
	{
		print_exported_list(shell);
		return (0);
	}
	i = 1;
	while (cmd->arg[i])
	{
		name = NULL;
		value = NULL;
		parse_arg_var(cmd->arg[i], &name, &value);
		if (handle_export_var(name, value, shell, status))
			status = 1;
		i++;
	}
	return (status);
}

static int	update_or_add_var(char *name, char *value, t_shell *shell)
{
	t_env_var	*exist;
	t_env_var	*new_var;

	exist = find_var(shell->env_list, name);
	if (exist)
	{
		if (value)
			update_var_value(exist, value);
		exist->exported = true;
	}
	else
	{
		new_var = create_var(name, value, true);
		if (new_var)
			add_var_to_list(&shell->env_list, new_var);
		else
			return (1);
	}
	return (0);
}

// function - 3
t_env_var	*find_var(t_env_var *env_list, char *name)
{
	if (!name)
		return (NULL);
	while (env_list)
	{
		if (ft_strcmp(env_list->name, name) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

// function - 4
void	add_var_to_list(t_env_var **env_list, t_env_var *new_var)
{
	t_env_var	*current;

	if (!env_list || !new_var)
		return ;
	if (*env_list == NULL)
		*env_list = new_var;
	else
	{
		current = *env_list;
		while (current->next)
			current = current->next;
		current->next = new_var;
	}
}

// function - 5
int	handle_export_var(char *name, char *value, t_shell *shell, int status)
{
	if (name == NULL || name[0] == '\0' || !is_name_valid(name))
	{
		name_invalid(name, value);
		status = 1;
	}
	else
	{
		status = update_or_add_var(name, value, shell);
	}
	free_name_value(name, value);
	return (status);
}
