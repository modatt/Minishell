/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:04:24 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 20:32:38 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, char *src, size_t size)
{
	size_t	x;

	x = 0;
	if (size > 0)
	{
		while (src[x] != '\0' && x < size - 1)
		{
			dest[x] = src[x];
			x++;
		}
		dest[x] = '\0';
	}
	ft_strlen(src);
	return (ft_strlen(src));
}
