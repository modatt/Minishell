/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:02:07 by modat             #+#    #+#             */
/*   Updated: 2024/09/30 16:05:18 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//void	ft_striteri(char *s, void(*f)(unsigned int, char *));
/*
void	passfunc(unsigned int i, char *s)
{
	if (s[i] >= 'a' && S[i] <= 'z')
		(s[i] - 32);
	else
		i++;
}
*/
void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	i;

	i = 0;
	if (!s)
		return ;
	while (s[i] != '\0')
	{
		(*f)(i, &s[i]);
		i++;
	}
}
