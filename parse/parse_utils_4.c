/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:31:26 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 17:54:58 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function for double quotes in handle_word_enhanced
void	process_double_quotes(char *line, int *i, char *result, int *result_len)
{
	(*i)++;
	while (line[*i] && line[*i] != 34 && *result_len < 1023)
	{
		result[(*result_len)++] = line[*i];
		(*i)++;
	}
	if (line[*i] == 34)
		(*i)++;
}

// Helper function for single quotes in handle_word_enhanced
void	process_single_quotes(char *line, int *i, char *result, int *result_len)
{
	(*i)++;
	while (line[*i] && line[*i] != 39 && *result_len < 1023)
	{
		result[(*result_len)++] = line[*i];
		(*i)++;
	}
	if (line[*i] == 39)
		(*i)++;
}

// Helper function to finalize token in handle_word_enhanced
void	finalize_token(char **tokens, int *k, char *result, int result_len)
{
	if (result_len > 0)
	{
		tokens[*k] = malloc(result_len + 1);
		if (!tokens[*k])
			return ;
		ft_strncpy(tokens[*k], result, result_len);
		tokens[*k][result_len] = '\0';
		(*k)++;
	}
}

// Helper function to parse quoted content for handle_double_qoute
int	parse_quoted_content(char *line, int *i, int *quoted_start)
{
	(*i)++;
	*quoted_start = *i;
	while (line[*i] && line[*i] != 34)
		(*i)++;
	return (*i - *quoted_start);
}

// Helper function to parse continuation for handle_double_qoute
int	parse_continuation(char *line, int *i, int *continuation_start)
{
	*continuation_start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != 34 && line[*i] != 39 && line[*i] != '<')
		(*i)++;
	return (*i - *continuation_start);
}