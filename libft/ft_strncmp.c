/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 10:19:44 by modat             #+#    #+#             */
/*   Updated: 2024/09/30 16:19:55 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>
/*
int	ft_strncmp(const char *s1, const char *s2, size_t n);

int	main(void)
{
	const char s1[] = "Abcdefgh";
	const char s2[] = "abcdwxyz";
	size_t n;
	size_t res;
	
	n = 4;
	res = ft_strncmp(s1, s2, n);
	printf("res: %ld", res);
	return (0);
}
*/
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] != '\0' && s2[i] && (i + 1 < n)
		&& (s1[i] == s2[i]))
	{
		i++;
	}
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}
