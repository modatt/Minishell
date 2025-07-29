/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   greets.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:29:42 by modat             #+#    #+#             */
/*   Updated: 2025/07/29 12:34:51 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_reset(void)
{
	printf("\033[0m");
}

void	greets_minishell(void)
{
	printf(GRN "__________________________________________________________\n");
	printf("\n");
	printf(PEACH "                WELCOME TO " WHITE "✨ MINISHELL ✨\n");
	printf(PEACH "                Done by");
	printf(GRN " : ");
	printf(WHITE "Hmeltaha");
	printf(GRN " & ");
	printf(WHITE "Modat\n");
	printf(GRN "__________________________________________________________\n");
	printf("\n");
	print_reset();
}
