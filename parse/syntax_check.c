/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:14:34 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 12:14:35 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1

int	count_words(char *str)
{
	int	i;
	int	wc;

	i = 0;
	wc = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (handle_pipe(str, &i) == 0)
		return (0);
	if ((str[i] == '>' && str[i + 1] && str[i + 1] == '>') || (str[i] == '<'
			&& str[i + 1] && str[i + 1] == '<'))
	{
		if (handle_redir_at_beg(str, &i, &wc) == 0)
			return (0);
	}
	if (str[i])
	{
		while (str[i])
		{
			if (str[i] == '|' && (!str[i + 1] || str[i + 1] == '|'))
			{
				write(1, "minishell: syntax error near unexpected token `|'\n",
					50);
				return (0);
			}
			count_qoute(str, &i, &wc);
			if (handle_syntax(str, &i, &wc) == 0)
				return (0);
			if (str[i] != '\0')
				i++;
			else
				break ;
		}
	}
	return (wc);
}

// function - 2
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

// function - 3
int	handle_redir_at_beg(char *str, int *i, int *wc)
{
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	if (handle_three_and_higher_redir(str, i) == 0)
		return (0);
	if (str[*i] == '>' || str[*i] == '<')
	{
		if (str[*i + 1] == str[*i])
		{
			(*i) += 2;
			while (str[*i] == ' ' || str[*i] == '\t')
				(*i)++;
			if (str[*i] != '\0')
			{
				(*wc) += 2;
				while (str[*i] && !((str[*i] == ' ' || str[*i] == '\t')))
					(*i)++;
				return (-1);
			}
			else
			{
				write(1, "bash: syntax error near unexpected token `newline'\n",
					51);
				return (0);
			}
		}
		else
		{
			(*i)++;
			while (str[*i] == ' ' || str[*i] == '\t')
				(*i)++;
			if (str[*i] == '\0')
			{
				write(1, "bash: syntax error near unexpected token `newline'\n",
					51);
				return (0);
			}
		}
	}
	return (-1);
}

// function - 4
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
		if (str[x] == '|' && !(str[*i] == '\0' || str[*i] == '|'))
		{
			write(1, "minishell: syntax error near unexpected token `|'\n", 50);
			return (0);
		}
	}
	return (-1);
}

// f unction - 5
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

// function - 6
int	handle_syntax(char *str, int *i, int *wc)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
		(*i)++;
	if (str[*i] && (str[*i] != 34 && str[*i] != 39) && (str[*i] != ' '
			|| str[*i] != '\t'))
	{
		if ((str[*i] == '>' || str[*i] == '<') && *wc == 0)
		{
			if (str[*i + 1] == str[*i])
			{
				(*i) += 2;
				while (str[*i] == ' ' || str[*i] == '\t')
					(*i)++;
				if (str[*i] == '\0' || str[*i] == '|')
				{
					write(1,
						"minishell: syntax error near unexpected token `newline'\n",
						56);
					return (0);
				}
			}
			else
			{
				(*i)++;
				while (str[*i] == ' ' || str[*i] == '\t')
					(*i)++;
				if (str[*i] == '\0' || str[*i] == '|' || str[*i] == '>'
					|| str[*i] == '<')
				{
					write(1,
						"minishell: syntax error near unexpected token `newline'\n",
						56);
					return (0);
				}
			}
			if (str[*i + 1] == '>' || str[*i + 1] == '<')
			{
				printf("minishell: syntax error near unexpected token `%c'\n",
					str[*i + 1]);
				return (0);
			}
		}
		while (str[*i] && !((str[*i] == ' ' || str[*i] == '\t'
					|| str[*i] == ')')))
			(*i)++;
		(*wc)++;
	}
	return (-1);
}
