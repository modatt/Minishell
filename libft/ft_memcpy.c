/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 11:20:28 by modat             #+#    #+#             */
/*   Updated: 2025/07/11 19:07:12 by marvin           ###   ########.fr       */
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

/*
Notes:
des can be same as src, for example: 
>> can be point to same memory 
memcpy(data + 1, data, 4);

char buffer[10] = "abcdef";
memcpy(buffer + 3, buffer, 3); // Copy into same array

or 
>> diferent/ seperated 
char *des[10];
char buffer[10] = "abcdef";
memcpy(des, buffer, 3); 

>> memcpy assume that you have a valid memory for the parameters.

>> in memory we do not terminate the string because we cpy only.

>> why there's not null terminated? 
✅ Because memcpy:
Works on raw memory (just bytes)

Has no idea whether it's copying a string, an array of ints, a struct, or anything else

Only copies exactly the number of bytes you tell it

It treats memory as just raw data — not C strings.
*/

