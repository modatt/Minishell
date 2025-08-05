/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:57:51 by modat             #+#    #+#             */
/*   Updated: 2025/08/05 13:08:43 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 3 
//Helper function to handle special tokens
static void	handle_redirection_tokens(char **tokens, int *k, char *line, int *i)
{
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		tokens[*k] = ft_strdup("<<");
		(*k)++;
		(*i) += 2;
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		tokens[*k] = ft_strdup(">>");
		(*k)++;
		(*i) += 2;
	}
	else if (line[*i] == '<')
	{
		tokens[*k] = ft_strdup("<");
		(*k)++;
		(*i)++;
	}
	else if (line[*i] == '>')
	{
		tokens[*k] = ft_strdup(">");
		(*k)++;
		(*i)++;
	}
}

// function - 2
static void	handle_pipe_token(char **tokens, int *k, char *line, int *i)
{
	if (line[*i] == '|')
	{
		tokens[*k] = ft_strdup("|");
		(*k)++;
		(*i)++;
	}
}

// function - 3
// Helper function to handle tokens
static void	handle_token(char **tokens, int *k, char *line, int *i)
{
	t_token_data	data;
	int				wbeg;

	data.tokens = tokens;
	data.k = k;
	data.line = line;
	data.i = i;
	data.wbeg = &wbeg;
	if (line[*i] == 34)
		handle_double_qoute(&data);
	else if (line[*i] == 39)
		handle_single_qoute(&data);
	else if (line[*i] == '<' || line[*i] == '>')
		handle_redirection_tokens(tokens, k, line, i);
	else if (line[*i] == '|')
		handle_pipe_token(tokens, k, line, i);
	else
		handle_word_enhanced(&data);
}

// function - 4
void	tokenizer2(char **tokens, int *k, int *i, char *line)
{
	while (line[*i])
	{
		while (line[*i] == ' ' || line[*i] == '\t')
			(*i)++;
		if (!line[*i])
			break ;
		handle_token(tokens, k, line, i);
	}
}
