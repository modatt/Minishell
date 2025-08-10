/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:54:58 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 20:32:09 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	n;

	n = ft_strlen(little);
	if (little[0] == '\0')
		return ((char *)big);
	while (*big && n <= len)
	{
		if (*big == *little && ft_strncmp(big, little, n) == '\0')
			return ((char *)big);
		++big;
		--len;
	}
	return (NULL);
}
