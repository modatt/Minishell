
#include "minishell.h"



char **copying_env(char **old_env)
{
	int	count = 0;
	int i;
	char **new_env;

	i = 0;
	while(old_env[i])
	{
		count++;
		i++;
	}
	new_env = malloc(sizeof(char *) * (count + 1));
	if(!new_env)
		return(NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(old_env[i]);
		i++;
	}
	new_env[i] = NULL;
	return(new_env);
}

char *get_env(char *value, char **envp)
{
	int i;
	int len;
	char *result;

	len = strlen(value);
	i = 0;
	while(envp[i])
	{
		if((ft_strncmp(value, envp[i],len) == 0) && (envp[i][len] == '='))
		{
			 result = ft_strdup(envp[i] + len + 1);
			 return(result);
		}
		i++;
	}
	result = ft_strdup("");
	return(result);
}

char	*expand_env_var(char *input, int *i)
{
	int j;
	char *value;

	j = 0;
	while (input[j] && (ft_isalnum(input[j]) || input[j] == '_'))
	{
		j++;
	}
	*i += j;
	value = ft_substr(input, 0, j);
	return(value);
}


 char *handle_question_mark(t_shell *shell, int *i)
{
    char *result;

    (*i)++;
    result = ft_itoa(shell->last_exit_status);
    (*i)++;
    return (result);
}

 char *handle_bash_parameter(t_shell *shell, int *i, char *input)
{
    int n;
    char *result;

    n = input[*i] - '0';
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
void free_env_copy(char **env_copy)
{
    int i = 0;

    if (!env_copy)
        return;

    while (env_copy[i])
    {
        free(env_copy[i]);
        i++;
    }
    free(env_copy);
}

char	*handle_single_quote(char *input, int *i)
{
	int		start;
	char	*value;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (input[*i] == '\'')
		(*i)++;
	return (value);
}

char	*expand_variables_in_token(char *input, t_shell *shell)
{
	int		i;
	char	*result;
	char	*value;
	char	*temp;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'') // handle single quotes
			value = handle_single_quote(input, &i);
		else if (input[i] == '$')
			value = handle_sign(input, shell, &i); // expand variables with shell context
		else
			value = char_to_str(input[i++]); // normal char
		temp = result;
		result = ft_strjoin(result, value);
		free(value);
		free(temp);
	}
	return (result);
}

void	free_tokens(char **tokens)
{
	int	i;

	if (!tokens)
		return;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}



char	**mvtokens_expanded(char **tokens, t_shell *shell)
{
	int		i;
	int		count;
	char	**new_tokens;

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
		i++;
	}
	new_tokens[i] = NULL;
	return (new_tokens);
}



// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell	shell;
// 	char	*tokens[] = {"'$HOME'", "$0", "$HOME$USER", "ls -l", "$HALA", NULL};
// 	char	**expanded;
// 	int		j;

// 	shell.argc = argc;
// 	shell.argv = argv;
// 	shell.envp = copying_env(envp);
// 	shell.last_exit_status = 42;

// 	expanded = tokens_expanded(tokens, &shell);
// 	if (!expanded)
// 		return (1);

// 	j = 0;
// 	while (expanded[j])
// 	{
// 		printf("Token %d: %s\n", j, expanded[j]);
// 		j++;
// 	}

// 	free_tokens(expanded);
// 	free_env_copy(shell.envp);
// 	return (0);
// }




