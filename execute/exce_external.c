/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exce_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:31:34 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/08/10 20:52:24 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// function - 1
void	print_command_not_found(t_command *cmd, t_shell *shell)
{
	write(2, "Command '", 9);
	write(2, cmd->arg[0], ft_strlen(cmd->arg[0]));
	write(2, "' not found\n", 12);
	shell->last_exit_status = 127;
	return ;
}

// function - 2
static void	parent_handle(pid_t pid, t_shell *shell)
{
	int	status;

	signal(SIGINT, handler_parent);
	signal(SIGQUIT, handler_parent_quit);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit_status = 128 + WTERMSIG(status);
	signals_prompt();
}

// function - 3
static	void	exec_child(t_command *cmd, t_shell *shell, char *resolved_path)
{
	signals_execution();
	if (cmd->redir_count)
		setup_redirection_fds(cmd);
	execve(resolved_path, cmd->arg, shell->envp);
	free(resolved_path);
	clean_exit(shell, 127, cmd);
}

// function - 4
void	exec_external(t_command *cmd, t_shell *shell)
{
	char	*resolved_path;
	pid_t	pid;

	resolved_path = get_resolved_path(cmd, shell);
	if (!resolved_path)
	{
		print_command_not_found(cmd, shell);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		shell->last_exit_status = 1;
		free(resolved_path);
		return ;
	}
	if (pid == 0)
		exec_child(cmd, shell, resolved_path);
	printf("hi\n");
	free(resolved_path);
	parent_handle(pid, shell);
}
