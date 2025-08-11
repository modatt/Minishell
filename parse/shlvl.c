/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:23:59 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/08/11 17:56:27 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_envp_array(t_shell *shell, const char *name, const char *value)
{
	int		i;
	int		key_len;
	char	*new_entry;
	char	*prefix;

	if (!name || !value || !shell->envp)
		return ;
	prefix = ft_strjoin(name, "=");
	if (!prefix)
		return ;
	key_len = ft_strlen(prefix);
	i = -1;
	while (shell->envp[++i])
	{
		if (ft_strncmp(shell->envp[i], prefix, key_len) == 0)
		{
			new_entry = ft_strjoin(prefix, value);
			if (!new_entry)
				break;
			free(shell->envp[i]);
			shell->envp[i] = new_entry;
			break;
		}
	}
	free(prefix);
}

//func ---1
void	update_shlvl(t_shell *shell)
{
	char	*shlvl_str;
	char	*new_val;
	int		shlvl;

	shlvl = 0;
	shlvl_str = get_envp(shell->envp, "SHLVL");
	if (!shlvl_str)
		return ;
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str) + 1;
	if (shlvl < 0 || shlvl > 1000)
		shlvl = 1;
	new_val = ft_itoa(shlvl);
	if (!new_val)
		return ;
	handle_export_var_cd("SHLVL", new_val, shell, 0);
	update_envp_array(shell, "SHLVL", new_val);
	free(new_val);
	free(shlvl_str);
}
