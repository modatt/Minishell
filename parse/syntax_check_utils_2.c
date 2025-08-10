/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:45:20 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 04:02:49 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
static int	print_newline_error_bash(void)
{
	write(1, "minishell: syntax error near unexpected token `newline'\n", 56);
	return (0);
}

// function - 2
static int	handle_double_redir_beg(char *s, int *i, int *wc)
{
	(*i) += 2;
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	if (!s[*i])
		return (print_newline_error_bash());
	(*wc) += 2;
	while (s[*i] && s[*i] != ' ' && s[*i] != '\t')
		(*i)++;
	return (-1);
}

// function - 3
static int	handle_single_redir_beg(char *s, int *i)
{
	(*i)++;
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	if (!s[*i])
		return (print_newline_error_bash());
	return (-1);
}

// function - 4
int	handle_redir_at_beg(char *s, int *i, int *wc)
{
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
	if (!handle_three_and_higher_redir(s, i))
		return (0);
	if (s[*i] == '>' || s[*i] == '<')
	{
		if (s[*i + 1] == s[*i])
			return (handle_double_redir_beg(s, i, wc));
		return (handle_single_redir_beg(s, i));
	}
	return (-1);
}

// function - 5
int	ft_isspace(char s)
{
	if (s == ' ' || s == '\t')
		return (1);
	return (0);
}
