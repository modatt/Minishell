/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 08:17:38 by modat             #+#    #+#             */
/*   Updated: 2025/08/08 15:32:20 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
char	*ft_strncpy(char *des, char *src, int n)
{
	int	i;

	i = -1;
	while (++i < n && src[i])
		des[i] = src[i];
	des[i] = '\0';
	return (des);
}

// function - 2
char	*ft_strcpy(char *des, char *src)
{
	int	i;

	if (!src)
		return (NULL);
	i = 0;
	while (src[i])
	{
		des[i] = src[i];
		i++;
	}
	des[i] = '\0';
	return (des);
}

// function - 3
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

// function - 4
t_command	*create_node(void)
{
	t_command	*new_node;

	new_node = malloc(sizeof(t_command));
	if (!new_node)
		return (NULL);
	init_command(new_node);
	return (new_node);
}

// function - 5
void	handle_signal_pipe(void)
{
	signal(SIGINT, handler_parent);
	signal(SIGQUIT, handler_parent_quit);
}
