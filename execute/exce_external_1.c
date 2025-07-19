#include "minishell.h"



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