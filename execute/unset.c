/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:30:31 by modat             #+#    #+#             */
/*   Updated: 2025/08/11 11:28:04 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_command *cmd, t_shell *shell)
{
	int		i;
	char	*name;

	i = 0;
	if (!cmd || !cmd->arg || !cmd->arg[1])
		return (-1);
	while (cmd->arg[i])
	{
		name = cmd->arg[i];
		if (is_name_valid(name) == true)
			remove_var(&shell->env_list, name);
		i++;
	}
	return (0);
}

void	remove_var(t_env_var **env_list, const char *name)
{
	t_env_var	*current;
	t_env_var	*prev;

	if (!env_list || !*env_list || !name)
		return ;
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
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	add_env_var(t_env_var **list, t_env_var *new)
{
	t_env_var	*cur;

	if (!*list)
	{
		*list = new;
		return ;
	}
	cur = *list;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}
