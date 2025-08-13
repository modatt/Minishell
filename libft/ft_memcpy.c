/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hala <hala@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 11:20:28 by modat             #+#    #+#             */
/*   Updated: 2025/08/10 20:30:44 by hala             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <ctype.h>
//#include <string.h>
//#include <stdio.h>

//void	*ft_memcpy(void *dest, const void *src, size_t n);
/*
int     main(void)
{
        char    src_str[] = "zyxwvutsrqponmlkjihgfedcba";
        char    dest_str[] = "hello";
        size_t n = 88;

        printf("%s\n",(char *)ft_memcpy(dest_str, src_str, n));
        printf("%s", dest_str);
        return 0;
}
*/
void	*ft_memcpy(void *dest_str, const void *src_str, size_t n)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	*src;

	ptr = (unsigned char *)dest_str;
	src = (unsigned char *)src_str;
	if (!dest_str && !src_str)
	{
		return (NULL);
	}
	i = 0;
	while (i < n)
	{
		ptr[i] = src[i];
		i++;
	}
	return (dest_str);
}
