/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:53:34 by modat             #+#    #+#             */
/*   Updated: 2025/08/14 18:00:31 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_command *cmd)
{
	int			i;
	t_command	*tmp;
	t_command	*cur;

	cur = cmd->head;
	while (cur)
	{
		if (cur->arg)
		{
			i = 0;
			while (cur->arg[i])
			{
				free(cur->arg[i]);
				i++;
			}
			free(cur->arg);
		}
		free_redir(cur);
		tmp = cur;
		cur = cur->next;
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
