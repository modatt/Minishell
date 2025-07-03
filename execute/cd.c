/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmeltaha <hmeltaha@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:43:02 by hmeltaha          #+#    #+#             */
/*   Updated: 2025/07/03 20:45:17 by hmeltaha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//function - 1
int     handle_export_var_cd(char *name, char *value, t_shell *shell, int status)
{
	if (name == NULL ||  name[0] == '\0' || !is_name_valid(name))
    {
		name_invalid(name, value);
        status = 1; // faild
    }
    else 
    { // no allocation because this is for search and find if var already exist
        t_env_var *exist = find_var(shell->env_list, name);
        if (exist)
        {
			if (value)
			update_var_value(exist, value);
            exist->exported = true;
        }
        else 
        {
			t_env_var *new_var = create_var(name, value, true);
            if (new_var)
			add_var_to_list(&shell->env_list, new_var);
            else
			status = 1;
        }
    }
    return status;
}
//function - 2
// we need a buffer to store the current path
static void export_env(t_shell *shell, char *name, char *value)
{
	if (!name || !value || !shell)
		return ;
	handle_export_var_cd((char *)name, (char *)value, shell, 1);
}

// function - 3
static int cd_check_args(t_command *cmd, t_shell *shell)
{
    int count;
    count = args_count(cmd->arg);
    if (count > 2)
    {
        printf("bash: cd: too many arguments\n");
        shell->last_exit_status = 1;
        return 1;
    }
    return 0;
}

//function - 4
static char *cd_get_target_path(t_command *cmd, t_shell *shell)
{
    if (args_count(cmd->arg) == 1)
    {
        char *home = get_envp(shell->envp, "HOME");
        if (!home)
        {
            printf("bash: cd: HOME not set\n");
            shell->last_exit_status = 1;
            return NULL;
        }
        return home;
    }
    return cmd->arg[1];
}

//function - 5
static int cd_update_pwd(t_shell *shell, char *old_pwd)
{
    char *new_pwd;
     new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        perror("bash: cd: getcwd failed");
        shell->last_exit_status = 1;
        free(old_pwd);
        return 1;
    }

    if (old_pwd)
        export_env(shell, "OLDPWD", old_pwd);
    export_env(shell, "PWD", new_pwd);

    free(old_pwd);
    free(new_pwd);
    return 0;
}

// function - 6
void builtin_cd(t_command *cmd, t_shell *shell)
{
    char *old_pwd;
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
        return ;
    if (cd_check_args(cmd, shell) == 1)
        return ;
    char *target = cd_get_target_path(cmd, shell);
    if (!target)
    {
        free(old_pwd);
        return ;
    }
    if (chdir(target) == -1)
    {
        perror("bash: cd");
        shell->last_exit_status = 1;
        free(old_pwd);
        return ;
    }
    if (cd_update_pwd(shell, old_pwd) == 1)
        return ;

    shell->last_exit_status = 0;
}
//function - 5





//void builtin_cd(t_command *cmd, t_shell *shell)
//{
//    int count;
//    char *old_pwd; // Get current path before cd
//    char *target;
//	char *new_pwd;
	
//	old_pwd = getcwd(NULL, 0);
//	count = args_count(cmd->arg);
//    if (count > 2)
//    {
//        printf("bash: cd: too many arguments\n");
//        shell->last_exit_status = 1;
//        free(old_pwd);
//        return;
//    }
//    if (count == 1)
//    {
//        target = get_envp(shell->envp, "HOME");
//        if (!target)
//        {
//            printf("bash: cd: HOME not set\n");
//            shell->last_exit_status = 1;
//            free(old_pwd);
//            return;
//        }
//    }
//    else
//        target = cmd->arg[1];
//    if (chdir(target) == -1) // Change directory
//    {
//        perror("bash: cd");
//        shell->last_exit_status = 1;
//        free(old_pwd);
//        return;
//    }
//   	new_pwd = getcwd(NULL, 0);
//    if (!new_pwd)
//    {
//        perror("bash: cd: getcwd failed");
//        shell->last_exit_status = 1;
//        free(old_pwd);
//        return;
//    }
//	//printf("here\n");
//	if (old_pwd)
//		export_env(shell, "OLDPWD", old_pwd);
//	if (new_pwd)
//    	export_env(shell, "PWD", new_pwd);
//    free(old_pwd);
//	free(new_pwd);
//	shell->last_exit_status = 0;
//}

