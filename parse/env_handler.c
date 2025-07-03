
#include "minishell.h"


// function - 1
static void	free_expanded_tokens(char **tokens, int last)
{
	while (--last >= 0)
		free(tokens[last]);
	free(tokens);
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
	count = 0;
	while (tokens[count])
		count++;
	new_tokens = malloc(sizeof(char *) * (count + 1));
	if (!new_tokens)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_tokens[i] = expand_variables_in_token(tokens[i], shell);
		if (!new_tokens[i])
			return (free_expanded_tokens(new_tokens, i), NULL);
		i++;
	}
	new_tokens[i] = NULL;
	return (new_tokens);
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
	while (env_list)
	{
		if (strcmp(env_list->name, key) == 0)
			return (ft_strdup(env_list->value));
		env_list = env_list->next;
	}
	return (ft_strdup(""));
}


// char	*get_env(char *value, char **envp)
// {
// 	int		i;
// 	int		len;
// 	char	*result;

// 	len = strlen(value);
// 	i = 0;
// 	while (envp[i])
// 	{
// 		if ((ft_strncmp(value, envp[i], len) == 0) && (envp[i][len] == '='))
// 		{
// 			result = ft_strdup(envp[i] + len + 1);
// 			return (result);
// 		}
// 		i++;
// 	}
// 	result = ft_strdup("");
// 	return (result);
// }


