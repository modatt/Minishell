#include "minishell.h"
#include <stdlib.h>

// This is a custom implementation of realloc as it's not in libft
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		if (old_size < new_size)
			ft_memcpy(new_ptr, ptr, old_size);
		else
			ft_memcpy(new_ptr, ptr, new_size);
		free(ptr);
	}
	return (new_ptr);
}

void handle_word(t_token_data *data)
{
    int result_len = 0;
    char result[1024];
    (void)data->wbeg;
    while (data->line[*data->i] && !ft_strchr(" \t|<>", data->line[*data->i]))
    {
        if (data->line[*data->i] == 34)
            process_double_quotes(data->line, data->i, result, &result_len);
        else if (data->line[*data->i] == 39)
            process_single_quotes(data->line, data->i, result, &result_len);
        else
        {
            if (result_len < 1023)
                result[result_len++] = data->line[*data->i];
            (*data->i)++;
        }
    }
    finalize_token(data->tokens, data->k, result, result_len);
}

void tokenizer2(char **tokens, int *k, int *i, char *line)
{
    int wbeg;
    t_token_data data;
    data.tokens = tokens;
    data.k = k;
    data.i = i;
    data.line = line;
    data.wbeg = &wbeg;
    while (line[*i])
    {
        while (line[*i] && ft_strchr(" \t", line[*i]))
            (*i)++;
        if (!line[*i])
            break;
        if (line[*i] == '|')
            handle_pipe(line, i);
        else if (line[*i] == '<')
            handle_less(line, i);
        else if (line[*i] == '>')
            handle_greater(line, i);
        else
            handle_word(&data);
    }
}

char **tokenizer(char *line)
{
    int i;
    int k;
    char **tokens;
    int wc;
    if (!line)
        return (NULL);
    i = 0;
    k = 0;
    wc = count_words(line);
    if (wc == 0)
        return (NULL);
    tokens = (char **)malloc(sizeof(char *) * (wc + 10));
    if (!tokens)
        return (NULL);
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    if (line[i])
        tokenizer2(tokens, &k, &i, line);
    tokens[k] = NULL;
    return (tokens);
}

// Minimal stub implementations to fix build
int handle_less(char *str, int *i) 
{ (void)str; 
	(*i)++; 
	return 0; 
}
int handle_greater(char *str, int *i) { (void)str; (*i)++; return 0; }
