/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:30 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 13:50:52 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*command_line;
	t_command	*cmd;
	t_shell		*shell;

	greets_minishell();
	shell = malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("Failed to allocate shell structure");
		return (1);
	}
	init_shell(shell, argc, argv, envp);
	if (!shell->envp)
		printf("Warning: Failed to initialize environment\n");
	signals_prompt();
	while (1)
	{
		g_heredoc_interrupted = 0;
		if (!isatty(0))
			dup2(2, 0);
		command_line = readline("minishell$ ");
		if (command_line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (*command_line)
			add_history(command_line);
		if (!*command_line)
		{
			free(command_line);
			continue ;
		}
		cmd = parser(command_line, shell);
		if (!cmd)
		{
			free(command_line);
			continue ;
		}
		print_command_list(cmd);
		if (cmd && (cmd->next != NULL || cmd->is_pipe))
			execute_pipeline(cmd, shell);
		else if (cmd)
			execute_cmd(cmd, shell);
		free_cmd(cmd);
		free(command_line);
	}
	clear_history();
	free_shell(shell);
	return (0);
}
