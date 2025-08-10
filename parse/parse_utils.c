/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:25:55 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 04:02:09 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// main function - 1
char	**tokenizer(char *line, t_shell *shell)
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
	{
		shell->last_exit_status = 2;
		return (NULL);
	}
	tokens = (char **)malloc(sizeof(char *) * (wc + 10));
	if (!tokens)
		return (NULL);
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i])
		tokenizer2(tokens, &k, &i, line);
	tokens[k] = NULL;
	return (tokens);
}

// function - 3 - Enhanced handle_word to support concatenation
void	handle_word_enhanced(t_token_data *data)
{
	int		result_len;
	char	result[1024];

	result_len = 0;
	(void)data->wbeg;
	while (data->line[*data->i] && data->line[*data->i] != ' '
		&& data->line[*data->i] != '\t' && data->line[*data->i] != '|'
		&& data->line[*data->i] != '<')
	{
		if (data->line[*data->i] == 34)
			process_double_quotes(data->line, data->i, result, &result_len);
		else if (data->line[*data->i] == 39)
			process_single_quotes(data->line, data->i, result, &result_len);
		else
		{
			if (result_len < 1023)
				result[result_len++] = data->line[*data->i];
			(*data->i)++;
		}
	}
	finalize_token(data->tokens, data->k, result, result_len);
}

// function - 4 - ORIGINAL handle_word (kept for compatibility)
void	handle_word(t_token_data *data)
{
	*data->wbeg = *data->i;
	while (data->line[*data->i] && data->line[*data->i] != ' '
		&& data->line[*data->i] != '\t' && data->line[*data->i] != '|'
		&& data->line[*data->i] != 34 && data->line[*data->i] != 39
		&& data->line[*data->i] != '<')
		(*data->i)++;
	if (*data->wbeg < *data->i)
	{
		data->tokens[*data->k] = (char *)malloc(sizeof(char) * (*data->i
					- *data->wbeg + 1));
		if (!data->tokens[*data->k])
			return ;
		ft_strncpy(data->tokens[*data->k], &data->line[*data->wbeg], (*data->i
				- *data->wbeg));
		data->tokens[*data->k][*data->i - *data->wbeg] = '\0';
		(*data->k)++;
	}
}
