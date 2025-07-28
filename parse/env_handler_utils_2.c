/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 12:07:37 by modat             #+#    #+#             */
/*   Updated: 2025/07/28 12:07:59 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function - 1
char	**copying_env(char **old_env)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	i = 0;
	while (old_env[i])
	{
		count++;
		i++;
	}
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(old_env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

// function - 2
char	*handle_sign(char *input, t_shell *shell, int *i)
{
	char	*value;
	char	*result;

	(*i)++;
	if (input[*i] == '?')
		result = handle_question_mark(shell, i);
	else if (input[*i] >= '0' && input[*i] <= '9')
		result = handle_bash_parameter(shell, i, input);
	else
	{
		value = expand_env_var(&input[*i], i);
		result = get_env(value, shell->env_list);
		free(value);
	}
	return (result);
}

// function - 3

char	*handle_bash_parameter(t_shell *shell, int *i, char *input)
{
	int		n;
	char	*result;

	n = input[*i] - '0';
	(*i)++;
	if (n < shell->argc)
		result = ft_strdup(shell->argv[n]);
	else
		result = ft_strdup("");
	return (result);
}

// function - 4
char	*expand_env_var(char *input, int *i)
{
	int		j;
	char	*value;

	j = 0;
	while (input[j] && (ft_isalnum(input[j]) || input[j] == '_'))
	{
		j++;
	}
	*i += j;
	value = ft_substr(input, 0, j);
	return (value);
}

// function - 5
char	*get_envp(char **envp, char *value)
{
	int		i;
	int		len;
	char	*result;

	len = strlen(value);
	i = 0;
	while (envp[i])
	{
		if ((ft_strncmp(value, envp[i], len) == 0) && (envp[i][len] == '='))
		{
			result = ft_strdup(envp[i] + len + 1);
			return (result);
		}
		i++;
	}
	result = ft_strdup("");
	return (result);
}
