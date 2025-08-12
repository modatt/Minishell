/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 08:07:34 by modat             #+#    #+#             */
/*   Updated: 2025/08/12 08:34:46 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// func - 1
static int	is_unequal_single_qoute(char *s)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == 39)
			count++;
		i++;
	}
	return (count);
}

// func - 2
static int	is_unequal_double_qoute(char *s)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == 34)
			count++;
		i++;
	}
	return (count);
}

// func - 3
int	handle_start(char *s, int *i, int *wc)
{
	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	if (!handle_pipe(s, i))
		return (0);
	if ((s[*i] == '>' && s[*i + 1] == '>') || (s[*i] == '<' && s[*i
				+ 1] == '<'))
		if (!handle_redir_at_beg(s, i, wc))
			return (0);
	if ((is_unequal_single_qoute(s) % 2) != 0)
	{
		write(2, "minishell: syntax error unclosed single quote '.\n", 49);
		return (0);
	}
	if ((is_unequal_double_qoute(s) % 2) != 0)
	{
		write(2, "minishell: syntax error unclosed double quote \".\n", 49);
		return (0);
	}
	return (1);
}
