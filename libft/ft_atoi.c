/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:35:58 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 20:31:03 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*#include <stdio.h>

int	ft_atoi(const char *nptr);

int	main(void)
{
	const char nptr[] = "+548";
	char res;
	
	res = ft_atoi(nptr);
	printf("res: %d", res);
	return (0);
}
	*/
int	ft_atoi(const char *nptr)
{
	int		result;
	int		sign;
	size_t	i;

	result = 0;
	i = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if ((nptr[i] == '+' && nptr[i + 1] != '-'))
		i++;
	if (nptr[i] == '-')
	{
		sign *= -1;
		i++;
	}
	while (nptr[i] != '\0' && nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + nptr[i] - '0';
		i++;
	}
	result *= sign;
	return (result);
}
