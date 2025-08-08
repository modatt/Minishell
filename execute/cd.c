/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:43:02 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/08/08 15:06:12 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
static void	export_env(t_shell *shell, char *name, char *value)
{
	if (!name || !value || !shell)
		return ;
	handle_export_var_cd((char *)name, (char *)value, shell, 1);
}

// function - 2
static int	cd_check_args(t_command *cmd, t_shell *shell)
{
	int	count;

	count = args_count(cmd->arg);
	if (count > 2)
	{
		printf("minishell: cd: too many arguments\n");
		shell->last_exit_status = 1;
		return (1);
	}
	return (0);
}

// function - 3
static char	*cd_get_target_path(t_command *cmd, t_shell *shell)
{
	char	*home;

	if (args_count(cmd->arg) == 1)
	{
		home = get_envp(shell->envp, "HOME");
		if (!home)
		{
			printf("minishell: cd: HOME not set\n");
			shell->last_exit_status = 1;
			return (NULL);
		}
		return (home);
	}
	return (cmd->arg[1]);
}

// function - 4
static int	cd_update_pwd(t_shell *shell, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("minishell: cd: getcwd failed");
		shell->last_exit_status = 1;
		free(old_pwd);
		return (1);
	}
	if (old_pwd)
		export_env(shell, "OLDPWD", old_pwd);
	export_env(shell, "PWD", new_pwd);
	free(old_pwd);
	free(new_pwd);
	return (0);
}

// function - 5
void	builtin_cd(t_command *cmd, t_shell *shell)
{
	char	*old_pwd;
	char	*target;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return ;
	if (cd_check_args(cmd, shell) == 1)
		return ;
	target = cd_get_target_path(cmd, shell);
	if (!target)
	{
		free(old_pwd);
		return ;
	}
	if (chdir(target) == -1)
	{
		perror("minishell: cd");
		shell->last_exit_status = 1;
		free(old_pwd);
		return ;
	}
	if (cd_update_pwd(shell, old_pwd) == 1)
	{
		free(target);
		return ;
	}
	if (args_count(cmd->arg) == 1)
	{
		free (target);
	}
	shell->last_exit_status = 0;
}
