/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:57:51 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 14:01:07 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

static void	handle_heredoc_token(char **tokens, int *k, int *i)
{
	tokens[*k] = malloc(sizeof(char) * 3);
	if (!tokens[*k])
		return ;
	tokens[*k][0] = '<';
	tokens[*k][1] = '<';
	tokens[*k][2] = '\0';
	(*k)++;
	(*i) += 2;
}

static void	handle_pipe_token(char **tokens, int *k, int *i)
{
	tokens[*k] = malloc(sizeof(char) * 2);
	if (!tokens[*k])
		return ;
	tokens[*k][0] = '|';
	tokens[*k][1] = '\0';
	(*k)++;
	(*i)++;
}

void	tokenizer2(char **tokens, int *k, int *i, char *line)
{
	int	wbeg;

	while (line[*i])
	{
		while (is_whitespace(line[*i]))
			(*i)++;
		if (!line[*i])
			break ;
		if (line[*i] == 34)
			handle_double_qoute(tokens, k, line, i, &wbeg);
		else if (line[*i] == 39)
			handle_single_qoute(tokens, k, line, i, &wbeg);
		else if (line[*i] == '<' && line[*i + 1] == '<')
			handle_heredoc_token(tokens, k, i);
		else if (line[*i] == '|')
			handle_pipe_token(tokens, k, i);
		else
			handle_word_enhanced(tokens, k, line, i, &wbeg);
	}
}
