/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/07/29 13:03:54 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*create_shell(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("Failed to allocate shell structure");
		return (NULL);
	}
	if ((init_shell(shell, argc, argv, envp) != 0))
	{
		printf("Failed to initialize shell\n");
		free(shell);
		return (NULL);
	}
	if (!shell->envp)
		printf("Warning: Failed to initialize environment\n");
	return (shell);
}

static int	handle_command(t_shell *shell, char *line)
{
	t_command	*cmd;

	cmd = parser(line, shell);
	if (!cmd)
		return (0);
	print_command_list(cmd);
	if (cmd->next || cmd->is_pipe)
		execute_pipeline(cmd, shell);
	else
		execute_cmd(cmd, shell);
	free_cmd(cmd);
	return (1);
}

void	main_loop(t_shell *shell)
{
	char	*command_line;

	while (1)
	{
		g_heredoc_interrupted = 0;
		command_line = readline("minishell$ ");
		if (!command_line)
		{
			printf("exit\n");
			break ;
		}
		if (*command_line)
			add_history(command_line);
		else
		{
			free(command_line);
			continue ;
		}
		if (!handle_command(shell, command_line))
		{
			free(command_line);
			continue ;
		}
		free(command_line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	greets_minishell();
	shell = create_shell(argc, argv, envp);
	if (!shell)
		return (1);
	signals_prompt();
	if (!isatty(0))
		dup2(2, 0);
	main_loop(shell);
	free_shell(shell);
	clear_history();
	return (0);
}
