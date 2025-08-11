/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:23:39 by modat             #+#    #+#             */
/*   Updated: 2025/08/11 10:10:12 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
static int	print_newline_error(void)
{
	write(1, "minishell: syntax error near unexpected token `newline'\n", 56);
	return (0);
}

// function - 2
static int	check_double_redir(char *str, int *i)
{
	(*i) += 2;
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
	if (str[*i] == '\0' || str[*i] == '|')
		return (print_newline_error());
	return (1);
}

// function - 3
static int	check_single_redir(char *str, int *i)
{
	(*i)++;
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
	if (str[*i] == '\0' || str[*i] == '|' || str[*i] == '>' || str[*i] == '<')
		return (print_newline_error());
	return (1);
}

// function - 4
static int	check_redirection_syntax(char *str, int *i)
{
	if (str[*i + 1] == str[*i])
	{
		if (!check_double_redir(str, i))
			return (0);
	}
	else if (!check_single_redir(str, i))
		return (0);
	if (str[*i + 1] == '>' || str[*i + 1] == '<')
	{
		printf("minishell: syntax error near unexpected token `%c'\n", str[*i
			+ 1]);
		return (0);
	}
	return (1);
}

// function - 5
int	handle_syntax(char *str, int *i, int *wc)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
	if (str[*i] && str[*i] != 34 && str[*i] != 39)
	{
		if ((str[*i] == '>' || str[*i] == '<') && *wc == 0)
		{
			if (!check_redirection_syntax(str, i))
				return (0);
		}
		while (str[*i] && str[*i] != ' ' && str[*i] != '\t' && str[*i] != ')')
			(*i)++;
		(*wc)++;
	}
	return (-1);
}
