/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:54:14 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 00:00:01 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// func ---1
void	init_env_list(t_shell *shell, char **envp)
{
	int			i;
	char		*name;
	char		*value;
	char		*equal;
	t_env_var	*new;

	i = 0;
	shell->env_list = NULL;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (!equal)
		{
			i++;
			continue ;
		}
		name = strndup(envp[i], equal - envp[i]);
		value = ft_strdup(equal + 1);
		new = new_env_var(name, value);
		if (!name || !value || !new)
		{
			free(name);
			free(value);
			i++;
			continue ;
		}
		add_env_var(&shell->env_list, new);
		free(name);
		free(value);
		i++;
	}
}

//func ---2
t_env_var	*new_env_var(char *name, char *value)
{
	t_env_var	*node;

	node = malloc(sizeof(t_env_var));
	if (!node)
		return (NULL);
	node->name = ft_strdup(name);
	node->value = ft_strdup(value);
	node->exported = true;
	node->next = NULL;
	return (node);
}

//func ---3
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
