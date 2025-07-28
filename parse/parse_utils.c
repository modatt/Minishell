/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:25:55 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 12:29:11 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// main function - 1
char	**tokenizer(char *line)
{
	int		i;
	int		k;
	char	**tokens;
	int		wc;

	if (!line)
		return (NULL);
	i = 0;
	k = 0;
	wc = count_words(line);
	if (wc == 0)
		return (NULL);
	tokens = (char **)malloc(sizeof(char *) * (wc + 10));
	if (!tokens)
		return (NULL);
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i])
	{
		tokenizer2(tokens, &k, &i, line);
	}
	tokens[k] = NULL;
	return (tokens);
}

// function - 2
void	tokenizer2(char **tokens, int *k, int *i, char *line)
{
	int	wbeg;

	while (line[*i])
	{
		while (line[*i] == ' ' || line[*i] == '\t')
			(*i)++;
		if (!line[*i])
			break ;
		if (line[*i] == 34)
		{
			handle_double_qoute(tokens, k, line, i, &wbeg);
		}
		else if (line[*i] == 39)
		{
			handle_single_qoute(tokens, k, line, i, &wbeg);
		}
		else if (line[*i] == '<' && line[*i + 1] == '<')
		{
			tokens[*k] = (char *)malloc(sizeof(char) * 3);
			if (!tokens[*k])
				return ;
			tokens[*k][0] = '<';
			tokens[*k][1] = '<';
			tokens[*k][2] = '\0';
			(*k)++;
			(*i) += 2;
		}
		else if (line[*i] == '|')
		{
			tokens[*k] = (char *)malloc(sizeof(char) * 2);
			if (!tokens[*k])
				return ;
			tokens[*k][0] = '|';
			tokens[*k][1] = '\0';
			(*k)++;
			(*i)++;
		}
		else if (line[*i] && line[*i] != ' ' && line[*i] != '\t')
		{
			handle_word_enhanced(tokens, k, line, i, &wbeg);
		}
	}
}

// function - 3 - Enhanced handle_word to support concatenation
void	handle_word_enhanced(char **tokens, int *k, char *line, int *i,
		int *wbeg)
{
	int	result_len;

	char result[1024];
	result_len = 0;
	(void)wbeg;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != '<')
	{
		if (line[*i] == 34)
		{
			(*i)++;
			while (line[*i] && line[*i] != 34 && result_len < 1023)
			{
				result[result_len++] = line[*i];
				(*i)++;
			}
			if (line[*i] == 34)
				(*i)++;
		}
		else if (line[*i] == 39) // Single quote
		{
			(*i)++; // Skip opening quote
			while (line[*i] && line[*i] != 39 && result_len < 1023)
			{
				result[result_len++] = line[*i];
				(*i)++;
			}
			if (line[*i] == 39) // Skip closing quote
				(*i)++;
		}
		else // Regular character
		{
			if (result_len < 1023)
				result[result_len++] = line[*i];
			(*i)++;
		}
	}
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

// function - 4 - ORIGINAL handle_word (kept for compatibility)
void	handle_word(char **tokens, int *k, char *line, int *i, int *wbeg)
{
	*wbeg = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != 34 && line[*i] != 39 && line[*i] != '<')
		(*i)++;
	if (*wbeg < *i)
	{
		tokens[*k] = (char *)malloc(sizeof(char) * (*i - *wbeg + 1));
		if (!tokens[*k])
			return ;
		ft_strncpy(tokens[*k], &line[*wbeg], (*i - *wbeg));
		tokens[*k][*i - *wbeg] = '\0'; // Null terminate
		(*k)++;
	}
}

// function - 4
void	handle_double_qoute(char **tokens, int *k, char *line, int *i,
		int *wbeg)
{
	int	continuation_start;
	int	continuation_len;

	int quoted_start, quoted_len, total_len = 0;
	(void)wbeg;
	(*i)++;
	quoted_start = *i;
	while (line[*i] && line[*i] != 34)
		(*i)++;
	quoted_len = *i - quoted_start;
	if (line[*i] == 34)
		(*i)++;
	continuation_start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != 34 && line[*i] != 39 && line[*i] != '<')
		(*i)++;
	continuation_len = *i - continuation_start;
	total_len = 2 + quoted_len + continuation_len + 1;
	tokens[*k] = (char *)malloc(sizeof(char) * total_len);
	if (!tokens[*k])
		return ;
	// Build the token: "quoted_content" + continuation
	tokens[*k][0] = 34; // Opening quote
	ft_strncpy(&tokens[*k][1], &line[quoted_start], quoted_len);
	tokens[*k][quoted_len + 1] = 34; // Closing quote
	if (continuation_len > 0)
		ft_strncpy(&tokens[*k][quoted_len + 2], &line[continuation_start],
			continuation_len);
	tokens[*k][total_len - 1] = '\0'; // Null terminate
	(*k)++;
}

// function - 5
void	handle_single_qoute(char **tokens, int *k, char *line, int *i,
		int *wbeg)
{
	int	continuation_start;
	int	continuation_len;

	int quoted_start, quoted_len, total_len = 0;
	(void)wbeg; // Suppress unused parameter warning
	// Process the quoted part
	(*i)++; // Skip opening quote
	quoted_start = *i;
	while (line[*i] && line[*i] != 39) // Find closing quote
		(*i)++;
	quoted_len = *i - quoted_start;
	if (line[*i] == 39) // Skip closing quote if present
		(*i)++;
	// Check if there are non-space characters immediately following
	continuation_start = *i;
	while (line[*i] && line[*i] != ' ' && line[*i] != '\t' && line[*i] != '|'
		&& line[*i] != 34 && line[*i] != 39 && line[*i] != '<')
		(*i)++;
	continuation_len = *i - continuation_start;
	// Calculate total length: quotes + quoted content + continuation + null
	total_len = 2 + quoted_len + continuation_len + 1;
	tokens[*k] = (char *)malloc(sizeof(char) * total_len);
	if (!tokens[*k])
		return ;
	// Build the token: 'quoted_content' + continuation
	tokens[*k][0] = 39; // Opening quote
	ft_strncpy(&tokens[*k][1], &line[quoted_start], quoted_len);
	tokens[*k][quoted_len + 1] = 39; // Closing quote
	if (continuation_len > 0)
		ft_strncpy(&tokens[*k][quoted_len + 2], &line[continuation_start],
			continuation_len);
	tokens[*k][total_len - 1] = '\0'; // Null terminate
	(*k)++;
}

// function - 6
char	*read_heredoc(char *delimiter)
{
	char	*line;
	char	*content;
	char	*temp;
	size_t	content_len;
	size_t	line_len;

	content = NULL;
	content_len = 0;
	while ((line = readline("> ")) != NULL)
	{
		// Check if line matches delimiter exactly
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		line_len = strlen(line);
		// Allocate space for new content + line + newline + null terminator
		temp = malloc(content_len + line_len + 2);
		if (!temp)
		{
			free(line);
			free(content);
			return (NULL);
		}
		// Copy existing content if any
		if (content)
		{
			strcpy(temp, content);
			free(content);
		}
		else
		{
			temp[0] = 0;
		}
		// Append new line with newline
		strcat(temp, line);
		strcat(temp, "\n");
		content = temp;
		content_len += line_len + 1;
		free(line);
	}
	return (content);
}
