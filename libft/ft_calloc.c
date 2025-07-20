/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:59:20 by modat             #+#    #+#             */
/*   Updated: 2024/09/30 15:22:19 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdint.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*ptr;
	size_t			i;
	size_t			totalsize;

	totalsize = nmemb * size;
	if (nmemb != 0 && size != 0 && nmemb > (SIZE_MAX / size))
	{
		return (NULL);
	}
	ptr = (unsigned char *)malloc(totalsize);
	if (ptr == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < totalsize)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *) ptr);
}
