/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:54:14 by modat             #+#    #+#             */
/*   Updated: 2025/08/11 09:57:31 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// func - 1
static size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	i;

	i = 0;
	while (i < maxlen && s[i] != '\0')
		i++;
	return (i);
}

// func - 2
static char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	size_t	i;
	char	*dup;

	i = 0;
	len = ft_strnlen(s, n);
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

// funct - 3
static void	add_env(t_shell *shell, char *env_str)
{
	char		*name;
	char		*value;
	char		*equal;
	t_env_var	*new;

	equal = ft_strchr(env_str, '=');
	if (!equal)
		return ;
	name = ft_strndup(env_str, equal - env_str);
	value = ft_strdup(equal + 1);
	new = new_env_var(name, value);
	if (!name || !value || !new)
	{
		free(name);
		free(value);
		return ;
	}
	add_env_var(&shell->env_list, new);
	free(name);
	free(value);
}

// func - 4
void	init_env_list(t_shell *shell, char **envp)
{
	int	i;

	i = 0;
	shell->env_list = NULL;
	while (envp[i])
	{
		add_env(shell, envp[i]);
		i++;
	}
}

// func - 5
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
