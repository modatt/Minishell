/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:35:58 by modat             #+#    #+#             */
/*   Updated: 2024/09/30 15:36:39 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <limits.h>
//#include <stdio.h>
/*
static	size_t	ft_count_digit(int n);
char	*ft_itoa(int n);

int	main(void)
{
	int	n = 123;
	char	*res;

	res = ft_itoa(n);
	printf("The result is: %s", res);
	free(res);
	return (0);
}
*/
static size_t	ft_count_digit(int n)
{
	size_t	count_digit;

	count_digit = 0;
	if (n == 0)
	{
		return (1);
	}
	if (n < 0)
	{
		count_digit++;
		n = -n;
	}
	while (n != 0)
	{
		n /= 10;
		count_digit++;
	}
	return (count_digit);
}

static	long	ifnegative(char *str, long nbr)
{
	if (nbr < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	return (nbr);
}

char	*ft_itoa(int n)
{
	size_t	count_digit;
	char	*str;
	long	nbr;

	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	count_digit = ft_count_digit(n);
	str = (char *)malloc((count_digit + 1) * (sizeof(char)));
	if (!str)
		return (NULL);
	str[count_digit] = '\0';
	if (n == 0)
	{
		str[0] = n + '0';
		return (str);
	}
	nbr = n;
	nbr = ifnegative(str, nbr);
	while (nbr > 0)
	{
		str[--count_digit] = (nbr % 10) + '0';
		nbr = nbr / 10;
	}
	return (str);
}
