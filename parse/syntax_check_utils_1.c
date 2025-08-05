/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:40:10 by modat             #+#    #+#             */
/*   Updated: 2025/08/05 11:37:19 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_start(char *s, int *i, int *wc)
{
	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	if (!handle_pipe(s, i))
		return (0);
	if ((s[*i] == '>' && s[*i + 1] == '>') || (s[*i] == '<'
			&& s[*i + 1] == '<'))
		if (!handle_redir_at_beg(s, i, wc))
			return (0);
	return (1);
}

static int	check_pipe_err(char *s, int i)
{
	if (s[i] == '|' && (!s[i + 1] || s[i + 1] == '|'))
	{
		write(1, "minishell: syntax error near unexpected token `|'\n", 50);
		return (0);
	}
	return (1);
}

static int	pipe_loop(char *s, int *i, int *wc)
{
	while (s[*i])
	{
		if (!check_pipe_err(s, *i))
			return (0);
		count_qoute(s, i, wc);
		if (!handle_syntax(s, i, wc))
			return (0);
		if (s[*i])
			(*i)++;
	}
	return (1);
}

int	count_words(char *s)
{
	int	i;
	int	wc;

	i = 0;
	wc = 0;
	if (!handle_start(s, &i, &wc))
		return (0);
	if (s[i] && !pipe_loop(s, &i, &wc))
		return (0);
	return (wc);
}
