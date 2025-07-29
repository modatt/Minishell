/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:11:03 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 17:31:20 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to build token for handle_double_qoute
void	build_double_quote_token(char **tokens, int *k, char *line,
		t_quote_data *data)
{
	int	total_len;

	total_len = 2 + data->quoted_len + data->continuation_len + 1;
	tokens[*k] = (char *)malloc(sizeof(char) * total_len);
	if (!tokens[*k])
		return ;
	tokens[*k][0] = 34;
	ft_strncpy(&tokens[*k][1], &line[data->quoted_start], data->quoted_len);
	tokens[*k][data->quoted_len + 1] = 34;
	if (data->continuation_len > 0)
		ft_strncpy(&tokens[*k][data->quoted_len + 2],
			&line[data->continuation_start], data->continuation_len);
	tokens[*k][total_len - 1] = '\0';
	(*k)++;
}

// Helper function to parse single quoted content
int	parse_single_quoted_content(char *line, int *i, int *quoted_start)
{
	(*i)++;
	*quoted_start = *i;
	while (line[*i] && line[*i] != 39)
		(*i)++;
	return (*i - *quoted_start);
}

// Helper function to parse continuation for handle_single_qoute
int	parse_single_continuation(char *line, int *i, int *continuation_start)
{
	*continuation_start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != 34 && line[*i] != 39 && line[*i] != '<')
		(*i)++;
	return (*i - *continuation_start);
}

// Helper function to build token for handle_single_qoute
void	build_single_quote_token(char **tokens, int *k, char *line,
		t_quote_data *data)
{
	int	total_len;

	total_len = 2 + data->quoted_len + data->continuation_len + 1;
	tokens[*k] = (char *)malloc(sizeof(char) * total_len);
	if (!tokens[*k])
		return ;
	tokens[*k][0] = 39;
	ft_strncpy(&tokens[*k][1], &line[data->quoted_start], data->quoted_len);
	tokens[*k][data->quoted_len + 1] = 39;
	if (data->continuation_len > 0)
		ft_strncpy(&tokens[*k][data->quoted_len + 2],
			&line[data->continuation_start], data->continuation_len);
	tokens[*k][total_len - 1] = '\0';
	(*k)++;
}
