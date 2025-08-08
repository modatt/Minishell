/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:53:34 by modat             #+#    #+#             */
/*   Updated: 2025/08/08 19:57:47 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_command *cmd)
{
	int			i;
	t_command	*tmp;

	while (cmd)
	{
		if (cmd->arg)
		{
			i = 0;
			while (cmd->arg[i])
			{
				free(cmd->arg[i]);
				i++;
			}
			free(cmd->arg);
		}
		free_redir(cmd);
		tmp = cmd;
		cmd = cmd->next;
		free(tmp);
	}
}

void	free_redir(t_command *cmd)
{
	int	i;

	i = 0;
	if (cmd->redirection)
	{
		while (i < cmd->redir_count)
		{
			if (cmd->redirection[i])
			{
				free(cmd->redirection[i]->file);
				free(cmd->redirection[i]);
			}
			i++;
		}
		free(cmd->redirection);
	}
}

void	free_shell(t_shell *shell)
{
	int	i;

	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
		{
			free(shell->envp[i]);
			shell->envp[i] = NULL;
			i++;
		}
		free(shell->envp);
		shell->envp = NULL;
	}
	free_env_list(shell);
	free(shell);
}

void	free_env_list(t_shell *shell)
{
	t_env_var	*tmp;

	while (shell->env_list)
	{
		tmp = shell->env_list;
		shell->env_list = shell->env_list->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}
