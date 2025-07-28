/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exce_external_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:49:49 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 13:04:51 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function --- 1
// Helper: join a directory and a command into dir/cmd
char	*join_path_cmd(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

// function --- 2
char	**get_path_dirs(t_shell *shell)
{
	char	*path_env;
	char	**dirs;

	path_env = get_envp(shell->envp, "PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	free(path_env);
	return (dirs);
}

// function --- 3
static char	*cleanup_and_return(char **dirs, char *ret)
{
	free_env_copy(dirs);
	return (ret);
}

// function --- 4
char	*find_cmd_in_path(char *arg, t_shell *shell)
{
	char	**dirs;
	char	*full_path;
	int		i;

	dirs = get_path_dirs(shell);
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full_path = join_path_cmd(dirs[i], arg);
		if (!full_path)
			return (cleanup_and_return(dirs, NULL));
		if (access(full_path, X_OK) == 0)
			return (cleanup_and_return(dirs, full_path));
		free(full_path);
		i++;
	}
	return (cleanup_and_return(dirs, NULL));
}

// function --- 5
char	*get_resolved_path(t_command *cmd, t_shell *shell)
{
	char	*resolved_path;

	if (ft_strchr(cmd->arg[0], '/'))
		resolved_path = ft_strdup(cmd->arg[0]);
	else
		resolved_path = find_cmd_in_path(cmd->arg[0], shell);
	if (!resolved_path)
	{
		print_command_not_found(cmd, shell);
		return (NULL);
	}
	return (resolved_path);
}
