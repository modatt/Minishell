/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:21:00 by modat             #+#    #+#             */
/*   Updated: 2025/07/29 17:32:19 by modat            ###   ########.fr       */
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
int	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	shell->argc = argc;
	shell->argv = argv;
	shell->last_exit_status = 0;
	shell->envp = copying_env(envp);
	shell->env_list = NULL;
	if (!shell->envp)
	{
		shell->env_list = NULL;
		return (1);
	}
	init_env_list(shell, envp);
	return (0);
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
		|| ft_strcmp(tokens[k], "<<") == 0
		|| ft_strcmp(tokens[k], "<") == 0);
}

// function - 5
void	handle_single_qoute(t_token_data *data)
{
	t_quote_data	quote_data;

	(void)data->wbeg;
	quote_data.quoted_len = parse_single_quoted_content(data->line, data->i,
			&quote_data.quoted_start);
	if (data->line[*data->i] == 39)
		(*data->i)++;
	quote_data.continuation_len = parse_single_continuation(data->line,
			data->i, &quote_data.continuation_start);
	build_single_quote_token(data->tokens, data->k, data->line, &quote_data);
}
