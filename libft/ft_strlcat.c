/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 10:55:39 by modat             #+#    #+#             */
/*   Updated: 2024/09/30 16:09:34 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char	*dst, const char *src, size_t size)
{
	size_t	x;
	size_t	y;
	size_t	to_copy;
	size_t	i;

	x = 0;
	y = 0;
	i = 0;
	if (!dst && !src)
		return (0);
	x = ft_strlen(src);
	y = ft_strlen(dst);
	if (size <= y)
		return (size + x);
	to_copy = size - y - 1;
	if (to_copy > x)
		to_copy = x;
	while (i < to_copy)
	{
		dst[y + i] = src[i];
		i++;
	}
	dst[y + to_copy] = '\0';
	return (y + x);
}
