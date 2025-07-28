/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:36:46 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/07/28 13:48:16 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
static int	is_within_long_limit(char *str, int start, int is_negative)
{
	char	*limit;
	int		i;

	if (is_negative)
		limit = "9223372036854775808";
	else
		limit = "9223372036854775807";
	i = 0;
	while (i < 19)
	{
		if (str[start + i] > limit[i])
			return (0);
		if (str[start + i] < limit[i])
			return (1);
		i++;
	}
	return (1);
}

// function - 2
int	is_numeric_i(char *str, int i)
{
	int	digit_count;
	int	is_negative;

	if (!str || !str[0])
		return (0);
	is_negative = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			is_negative = 1;
		i++;
	}
	if (!str[i])
		return (0);
	digit_count = 0;
	while (str[i + digit_count])
	{
		if (!ft_isdigit(str[i + digit_count]))
			return (0);
		digit_count++;
	}
	if (digit_count > 19 || (digit_count == 19 && !is_within_long_limit(str, i,
				is_negative)))
		return (0);
	return (1);
}

// function - 3
int	is_numeric(char *str)
{
	return (is_numeric_i(str, 0));
}

// function - 4
int	args_count(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}
