/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:40:10 by modat             #+#    #+#             */
/*   Updated: 2025/08/08 13:29:10 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1 
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

// function - 2
static int	check_pipe_err(char *s, int i)
{
	if (s[i] == '|')
	{
		if (s[i] == '|' && (!s[i + 1] || s[i + 1] == '|'))
		{
			write(2, "minishell: syntax error near unexpected token `||'\n", 51);
			return (0);
		}
		i++;
		while (ft_isspace(s[i]))
			i++;
		if (s[i] == '|')
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
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
	if (s[i] && !pipe_loop(s, &i, &wc))
		return (0);
	//if (!handle_med_and_end())
	return (wc);
}

// function - 5
int 	ft_isspace(char s)
{
	if (s == ' ' || s == '\t')
		return (1);
	return (0);
}
