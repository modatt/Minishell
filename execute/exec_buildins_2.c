#include "minishell.h"


// PATH_MAX = is a constant defiend in <limits.h> that represnt the max num of char in a full absulat file path
// we need a buffer to store the current path

// function - 1
void	builtin_echo(t_command *cmd)
{
	int	i;

	if (!cmd->arg[1])
	{
		printf("\n");
		return ;
	}
	i = 1;
	while (cmd->arg[i] && (ft_strcmp(cmd->arg[i], "-n") == 0))
		i++;
	while (cmd->arg[i])
	{
		printf("%s", cmd->arg[i]);
		i++;
		if (cmd->arg[i])
			printf(" ");
	}
	if (ft_strcmp(cmd->arg[1], "-n"))
		printf("\n");
}


// function - 2
void	builtin_pwd(t_command *cmd)
{
	char	path[PATH_MAX + 1];

	(void)cmd;
	if (getcwd(path, sizeof(path)))
		printf("%s \n", path);
	return ;
}


// function - 3
void 	builtin_env(t_shell *shell)
{
	t_env_var *current;

	current = shell->env_list;
	while(current)
	{
		if (current->value)
			printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}
//PATH_MAX = is a constant defiend in <limits.h> that represnt the max num of char in a full absulat file path 
// we need a buffer to store the current path


// function - 4
// void update_pwd_env(t_command *cmd, t_shell *shell)
// {
//     char *new_pwd;

//     new_pwd = getcwd(NULL, 0);
//     if (!new_pwd)
//         return ;
//     // unset_env(shell, "OLDPWD");
//     export_env(shell, "OLDPWD", shell->env_list->value);
//     unset_env(shell, "PWD");
//     export_env(shell, "PWD", new_pwd);
//     shell->last_exit_status = 0;
//     free(new_pwd);
// }


// // function - 5
// void builtin_cd(t_command *cmd, t_shell *shell)
// {
//    char *old_pwd;
//    char *target;

//    old_pwd = getcwd(NULL, 0);
//    if (cmd->arg[2])
//    {
//        printf("bash: cd: too many arguments\n");
//        shell->last_exit_status = 1;
//        free(old_pwd);
//        return ;
//    }
//    target = NULL;
//    if (!cmd->arg[1])
//        target = get_env(shell, "HOME");
//    else
//        target = cmd->arg[1];
//    if (chdir(target) == -1)
//    {
//        perror("bash: cd: No such file or directory");
//        shell->last_exit_status = 1;
//        free(old_pwd);
//        return ;
//    }
//    update_pwd_env(cmd, shell);
//    free(old_pwd);
// }
