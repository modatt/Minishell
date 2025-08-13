/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:11:53 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 20:33:05 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*b;
	unsigned char	*d;

	i = 0;
	b = (unsigned char *)s1;
	d = (unsigned char *)s2;
	while (i < n)
	{
		if (b[i] != d[i])
		{
			return (b[i] - d[i]);
		}
		i++;
	}
	return (0);
}
