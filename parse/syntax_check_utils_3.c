/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 08:07:34 by modat             #+#    #+#             */
/*   Updated: 2025/08/13 16:36:39 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// func - 1
static int	is_unequal_single_qoute(char *s)
{
	int	i;
	int	count;
	int	in_double;

	i = 0;
	count = 0;
	in_double = 0;
	while (s[i])
	{
		if (s[i] == '"' && !count)
			in_double = !in_double;
		else if (s[i] == '\'' && !in_double)
			count++;
		i++;
	}
	return (count);
}

// func - 2
static int	is_unequal_double_qoute(char *s)
{
	int	i;
	int	count;
	int	in_single;

	i = 0;
	count = 0;
	in_single = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !count)
			in_single = !in_single;
		else if (s[i] == '"' && !in_single)
			count++;
		i++;
	}
	return (count);
}

static int	is_single_pipe(char *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == '|')
			count++;
		i++;
	}
	if (count == 1)
		return (1);
	return (0);
}

// func - 3
int	handle_start(char *s, int *i, int *wc)
{
	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	if (is_single_pipe(s) != 1)
	{
		if (!handle_pipe(s, i))
			return (0);
		if ((s[*i] == '>' && s[*i + 1] == '>') || (s[*i] == '<' && s[*i
					+ 1] == '<'))
			if (!handle_redir_at_beg(s, i, wc))
				return (0);
	}
	if ((is_unequal_double_qoute(s) % 2) != 0)
	{
		write(2, "minishell: syntax error unclosed double quote \".\n", 49);
		return (0);
	}
	if ((is_unequal_single_qoute(s) % 2) != 0)
	{
		write(2, "minishell: syntax error unclosed single quote '.\n", 49);
		return (0);
	}
	return (1);
}
