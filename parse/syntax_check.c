/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:14:34 by modat             #+#    #+#             */
/*   Updated: 2025/08/07 14:24:37 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
int	handle_three_and_higher_redir(char *str, int *i)
{
	if (str[*i + 2] == '>' || str[*i + 2] == '<')
	{
		if (str[*i + 1] == str[*i])
		{
			if (str[*i + 2] == str[*i + 3])
				printf("minishell: syntax error near unexpected token `%c%c'\n",
					str[*i + 2], str[*i + 3]);
			else
				printf("minishell: syntax error near unexpected token `%c'\n",
					str[*i + 2]);
			return (0);
		}
		else
		{
			if (str[*i + 2] == str[*i + 1])
				printf("minishell: syntax error near unexpected token `%c%c'\n",
					str[*i + 2], str[*i + 1]);
			else
				printf("minishell: syntax error near unexpected token `%c'\n",
					str[*i + 2]);
			return (0);
		}
	}
	return (-1);
}

// function - 2
int	handle_pipe(char *str, int *i)
{
	int	x;

	if (str[*i] == '|')
	{
		x = *i;
		if (str[*i] == '|' && (str[*i + 1] == '\0' || str[*i + 1] == '|'))
		{
			write(1, "minishell: syntax error near unexpected token `|'\n", 50);
			return (0);
		}
		(*i)++;
		while (str[*i] == ' ' || str[*i] == '\t')
			(*i)++;
		if (str[x] == '|' && !(str[*i + 1] == '\0' || str[*i + 1] == '|'))
		{
			write(1, "minishell: syntax error near unexpected token `|'\n", 50);
			return (0);
		}
		if (str[*i] == '\0' || str[*i] == '|')
		{
			write(1, "minishell: syntax error near unexpected token `|'\n", 50);
			return (0);
		}
	}
	return (1);
}

// function - 3
void	count_qoute(char *str, int *i, int *wc)
{
	if (str[*i] && str[*i] == 34)
	{
		(*i)++;
		while (str[*i] && str[*i] != 34)
			(*i)++;
		(*i)++;
		(*wc)++;
	}
	if (str[*i] && str[*i] == 39)
	{
		(*i)++;
		while (str[*i] && str[*i] != 39)
			(*i)++;
		(*i)++;
		(*wc)++;
	}
}
