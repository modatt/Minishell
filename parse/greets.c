/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   greets.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:29:42 by modat             #+#    #+#             */
/*   Updated: 2025/05/31 17:30:22 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	greets_minishell(void)
{
	printf(GRN "__________________________________________________________\n" RESET);
	printf("\n");
	printf(PEACH "                WELCOME TO " WHITE "✨ MINISHELL ✨\n" RESET);
	printf(PEACH "                Done by" GRN ": " WHITE "Hmeltaha" GRN " & " WHITE "Modat\n" RESET);
	printf(GRN "__________________________________________________________\n" RESET);
	printf("\n");
}
