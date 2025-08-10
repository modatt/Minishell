/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:40:10 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 20:41:08 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_med_and_end(char *s, int i)
{
	while (s[i])
	{
		if (s[i] == '|' && ft_isalpha(s[i - 1]) && s[i + 1] == '|')
		{
			write(2, "minishell: syntax error near", 28);
			write(2, " unexpected token `||'\n", 23);
			return (0);
		}
		else if (s[i] == '|' && ft_isalpha(s[i - 1]))
		{
			write(2, "minishell: syntax error near", 28);
			write(2, " unexpected token `|'\n", 22);
			return (0);
		}
		i++;
	}
	return (1);
}

// function - 1
static int	handle_start(char *s, int *i, int *wc)
{
	while (s[*i] && (s[*i] == ' ' || s[*i] == '\t'))
		(*i)++;
	if (!handle_pipe(s, i))
		return (0);
	if ((s[*i] == '>' && s[*i + 1] == '>')
		|| (s[*i] == '<' && s[*i + 1] == '<'))
		if (!handle_redir_at_beg(s, i, wc))
			return (0);
	return (1);
}

// function - 2
static int	check_pipe_err(char *s, int i)
{
	if (s[i] == '|')
	{
		if (s[i] == '|' && (!s[i + 1] || s[i + 1] == '|'))
		{
			write(2, "minishell: syntax error near", 28);
			write(2, " unexpected token `||'\n", 23);
			return (0);
		}
		i++;
		while (ft_isspace(s[i]))
			i++;
		if (s[i] == '|')
		{
			write(2, "minishell: syntax error near", 28);
			write(2, " unexpected token `|'\n", 22);
			return (0);
		}
	}
	return (1);
}

// function - 3
static int	pipe_loop(char *s, int *i, int *wc)
{
	if (!s)
		return (0);
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

// function - 4
int	count_words(char *s)
{
	int	i;
	int	wc;

	i = 0;
	wc = 0;
	if (!handle_start(s, &i, &wc))
		return (0);
	if (!handle_med_and_end(s, i))
		return (0);
	if (s[i] && !pipe_loop(s, &i, &wc))
		return (0);
	return (wc);
}
