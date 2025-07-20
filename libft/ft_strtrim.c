/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:02:07 by modat             #+#    #+#             */
/*   Updated: 2024/09/30 16:29:32 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	total;
	char	*dest;

	i = 0;
	total = 0;
	if (!s1 || !set)
		return (NULL);
	total = ft_strlen(s1);
	while (s1[i] && ft_strchr(set, s1[i]))
	{
		i++;
	}
	while (total > i && ft_strchr(set, s1[total - 1]))
	{
		total--;
	}
	dest = (char *)malloc((total - i + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	ft_memcpy(dest, s1 + i, total - i);
	dest[total - i] = '\0';
	return (dest);
}
