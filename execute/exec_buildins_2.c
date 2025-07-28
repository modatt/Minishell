/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_buildins_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:47:51 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 13:13:47 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function --- 1
static int	is_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

// function --- 2
void	builtin_echo(t_command *cmd)
{
	int	i;
	int	no_newline;

	no_newline = 0;
	if (!cmd->arg[1])
	{
		printf("\n");
		return ;
	}
	i = 1;
	while (cmd->arg[i] && is_n_flag(cmd->arg[i]))
	{
		no_newline = 1;
		i++;
	}
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
void	builtin_env(t_shell *shell)
{
	t_env_var	*current;

	current = shell->env_list;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}

// function ---5
int	handle_export_var_cd(char *name, char *value, t_shell *shell, int status)
{
	t_env_var	*exist;
	t_env_var	*new_var;

	if (!name || !name[0] || !is_name_valid(name))
	{
		name_invalid(name, value);
		return (1);
	}
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
		if (!new_var)
			return (1);
		add_var_to_list(&shell->env_list, new_var);
	}
	return (status);
}
