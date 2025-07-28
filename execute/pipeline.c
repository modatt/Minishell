/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 20:49:04 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 00:06:01 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
static void	run_pipeline_loop(t_command *cmd_list, t_shell *shell,
		t_pipeline_data *data)
{
	t_command	*current_cmd;
	int			pipe_fds[2];

	current_cmd = cmd_list;
	while (current_cmd != NULL)
	{
		if (!setup_pipeline_execution(current_cmd, shell, data, pipe_fds))
			return ;
		current_cmd = current_cmd->next;
	}
}

// function - 2
void	execute_pipeline(t_command *cmd_list, t_shell *shell)
{
	int				prev_pipe_rd_fd;
	int				pid_count;
	t_pipeline_data	data;

	init_pipeline_data(cmd_list, &data, &prev_pipe_rd_fd, &pid_count);
	if (!data.child_pids)
		return ;
	run_pipeline_loop(cmd_list, shell, &data);
	wait_for_children(data.child_pids, pid_count, shell);
	free(data.child_pids);
}
