/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:55:00 by modat             #+#    #+#             */
/*   Updated: 2025/08/07 15:27:16 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// func 1
static int	get_command_line(t_shell *shell, char **line)
{
	if (shell->is_interactive)
		return (interactive(shell, line));
	else
		return (non_interactive(shell, line));
}

// func 2
static int	should_continue_loop(int status)
{
	if (status == -1)
		return (INPUT_RETRY);
	if (status == 0)
		return (INPUT_EOF);
	return (INPUT_OK);
}

// func 3
int	process_command(t_shell *shell, char *line)
{
	if (!line || line[0] == '\0')
		return (0);
	if (handle_command(shell, line) == 0)
	{
		free(line);
		return (0);
	}
	return (1);
}

// func 4
static void	set_signal_status(t_shell *shell)
{
	if (g_signal_status == 130)
	{
		g_signal_status = 0;
		shell->last_exit_status = 130;
	}
}

// func 5
void	main_loop(t_shell *shell)
{
	char	*command_line;
	int		status;
	int		check;

	while (1)
	{
		if (shell->is_interactive)
			signals_prompt();
		else
			setup_non_interactive_signals();
		g_signal_status = 0;
		status = get_command_line(shell, &command_line);
		set_signal_status(shell);
		check = should_continue_loop(status);
		if (check == INPUT_EOF)
		{
			printf("exit\n");
			break ;
		}
		if (check == INPUT_RETRY)
			continue ;
		handle_input_status(shell, status, command_line);
		set_signal_status(shell);
		free(command_line);
	}
}
