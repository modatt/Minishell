/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:10:12 by modat             #+#    #+#             */
/*   Updated: 2025/08/11 15:07:19 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
static void	free_expanded_tokens(char **tokens, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(tokens[i]);
		i++;
	}
}

// function - 2
static int	update_quote_state(char c, t_quote_state *state)
{
	if (c == '\'' && !state->in_double)
	{
		state->in_single = !state->in_single;
		return (1);
	}
	if (c == '\"' && !state->in_single)
	{
		state->in_double = !state->in_double;
		return (1);
	}
	return (0);
}

// function - 3

char	**tokens_expanded(char **tokens, t_shell *shell)
{
	int		i;
	int		count;
	char	**new_tokens;

	if (!tokens)
		return (NULL);
	count = args_count(tokens);
	new_tokens = malloc(sizeof(char *) * (count + 1));
	if (!new_tokens)
		return (free_tokens(tokens), NULL);
	i = -1;
	while (++i < count)
	{
		new_tokens[i] = expand_variables_in_token(tokens[i], shell);
		if (!new_tokens[i])
		{
			free_expanded_tokens(new_tokens, i);
			free(new_tokens);
			return (free_tokens(tokens), NULL);
		}
	}
	new_tokens[i] = NULL;
	return (free_tokens(tokens), new_tokens);
}

// function - 4
char	*expand_variables_in_token(char *input, t_shell *shell)
{
	int				i;
	char			*result;
	char			*value;
	t_quote_state	state;

	i = 0;
	state.in_single = 0;
	state.in_double = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (update_quote_state(input[i], &state))
		{
			i++;
			continue ;
		}
		if (input[i] == '$' && !state.in_single)
			value = handle_sign(input, shell, &i);
		else
			value = char_to_str(input[i++]);
		result = ft_strjoin_free(result, value);
	}
	return (result);
}

// function - 5
char	*get_env(char *key, t_env_var *env_list)
{
	char	*result;

	while (env_list)
	{
		if (ft_strcmp(env_list->name, key) == 0)
		{
			result = ft_strdup(env_list->value);
			if (result == NULL)
				return (ft_strdup(""));
			else
				return (result);
		}
		env_list = env_list->next;
	}
	return (ft_strdup(""));
}
