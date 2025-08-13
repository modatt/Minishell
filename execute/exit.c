/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:47:24 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/08/11 13:42:56 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
long	ft_atol(const char *nptr)
{
	long	result;
	int		i;
	int		sign;

	i = 0;
	sign = 1;
	result = 0;
	while ((nptr[i] == ' ') || ((nptr[i] >= 9) && (nptr[i] <= 13)))
		i++;
	if ((nptr[i] == '+') || (nptr[i] == '-'))
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while ((nptr[i] >= '0') && (nptr[i] <= '9'))
	{
		result = result * 10 + nptr[i] - '0';
		i++;
	}
	return (result * sign);
}

// function - 2
void	clean_exit(t_shell *shell, long exit_code, t_command *cmd)
{
	(void)cmd;
	if (!shell)
		exit(exit_code);
	free_cmd(cmd);
	free_shell(shell);
	exit(exit_code);
}

// function - 3
static void	print_numric_error(char *arg)
{
	write(2, "minishell: exit: ", 17);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument required\n", 28);
}

// function - 4
void	builtin_exit(t_command *cmd, t_shell *shell)
{
	int	count;

	count = args_count(cmd->arg);
	printf("exit\n");
	if (count == 2)
	{
		if (!(is_numeric(cmd->arg[1])))
		{
			print_numric_error(cmd->arg[1]);
			clean_exit(shell, 2, cmd);
		}
		clean_exit(shell, ft_atol((const char *)cmd->arg[1]), cmd);
	}
	if (count > 2)
	{
		if (!is_numeric(cmd->arg[1]))
		{
			print_numric_error(cmd->arg[1]);
			clean_exit(shell, 2, cmd);
		}
		write(2, "bash: exit: too many arguments\n", 31);
		shell->last_exit_status = 1;
		return ;
	}
	clean_exit(shell, shell->last_exit_status, cmd);
}
