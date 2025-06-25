
#include "minishell.h"

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
void	free_env_copy(char **env_copy)
{
	int	i;

	i = 0;
	if (!env_copy)
		return ;
	while (env_copy[i])
	{
		free(env_copy[i]);
		i++;
	}
	free(env_copy);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

static void	free_expanded_tokens(char **tokens, int last)
{
	while (--last >= 0)
		free(tokens[last]);
	free(tokens);
}
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
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	*get_env(char *value, char **envp)
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

char	*handle_question_mark(t_shell *shell, int *i)
{
	char	*result;

	(*i)++;
	result = ft_itoa(shell->last_exit_status);
	(*i)++;
	return (result);
}

char	*handle_bash_parameter(t_shell *shell, int *i, char *input)
{
	int		n;
	char	*result;

	n = input[*i] - '0'; // convert it to integer
	(*i)++;
	if (n < shell->argc)
		result = ft_strdup(shell->argv[n]);
	else
		result = ft_strdup("");
	return (result);
}

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
		result = get_env(value, shell->envp);
		free(value);
	}
	return (result);
}

char	*char_to_str(char c)
{
	char	*s;

	s = malloc(2);
	if (!s)
		return (NULL);
	s[0] = c;
	s[1] = '\0';
	return (s);
}


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
