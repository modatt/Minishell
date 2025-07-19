
#include "minishell.h"


// function - 1
static void free_expanded_tokens(char **tokens, int count)
{
    int i = 0;
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

char **tokens_expanded(char **tokens, t_shell *shell)
{
    int     i = 0;
    int     count = 0;
    char    **new_tokens;

    if (!tokens)
        return NULL;

    while (tokens[count])
        count++;

    new_tokens = malloc(sizeof(char *) * (count + 1));
    if (!new_tokens)
        return NULL;

    while (i < count)
    {
        new_tokens[i] = expand_variables_in_token(tokens[i], shell);
        if (!new_tokens[i])
        {
            free_expanded_tokens(new_tokens, i); // free built strings
            free(new_tokens);                    // free pointer array
            free_tokens(tokens);                 // ✅ free original tokens
            return NULL;                         // return NULL instead of tokens
        }
        i++;
    }

    new_tokens[i] = NULL;
    free_tokens(tokens);  // ✅ free old tokens
    return new_tokens;    // caller must later free_tokens(new_tokens)
}


//// function - 4
//char *expand_variables_in_token(char *input, t_shell *shell)
//{
//    int i = 0;
//    char *result = ft_strdup("");
//    char *value;
//    t_quote_state state = {0,0};

//    if (!result)
//        return NULL;
//    while (input[i])
//    {
//        if (update_quote_state(input[i], &state)) {
//            i++;
//            continue;
//        }
//        if (input[i] == '$' && !state.in_single)
//            value = handle_sign(input, shell, &i);
//        else
//            value = char_to_str(input[i++]);
//        if (!value) 
//		{
//            free(result);
//            return NULL;  // prevents leaking partial value
//        }
//        char *tmp = ft_strjoin_free(result, value);
//        if (!tmp) 
//		{
//            free(value);   // free partial allocation
//            free(result);  // free accumulated string
//            return NULL;   // prevent leak
//        }
//        result = tmp;
//    }
//    return result;
//}

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
		if (ft_strcmp(env_list->name, key) == 0)
			return (ft_strdup(env_list->value));
		env_list = env_list->next;
	}
	return (ft_strdup(""));
}





