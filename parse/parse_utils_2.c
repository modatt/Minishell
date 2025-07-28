/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:21:00 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 12:21:12 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
void	allocate_memory_shell(t_shell **shell)
{
	*shell = malloc(sizeof(t_shell));
	if (!(*shell))
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
}

// function - 2
void	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	shell->argc = argc;
	shell->argv = argv;
	shell->last_exit_status = 0;
	shell->envp = copying_env(envp);
	shell->env_list = NULL;
	if (!shell->envp)
	{
		shell->env_list = NULL;
		return ;
	}
	init_env_list(shell, envp);
}

// function - 3
void	init_command(t_command *cmd)
{
	cmd->arg = NULL;
	cmd->is_pipe = false;
	cmd->redirection = NULL;
	cmd->redir_count = 0;
	cmd->next = NULL;
}

// function - 4
bool	is_redirector(char **tokens, int k)
{
	return (ft_strcmp(tokens[k], ">>") == 0 || ft_strcmp(tokens[k], ">") == 0
		|| ft_strcmp(tokens[k], "<<") == 0 || ft_strcmp(tokens[k], "<") == 0);
}

// function - 5
