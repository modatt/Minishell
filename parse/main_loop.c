#include "minishell.h"
#define INPUT_EOF     0  // Exit the main loop
#define INPUT_OK      1  // Proceed with parsing and execution
#define INPUT_RETRY   2  // Continue loop without parsing

//func 1
static int get_command_line(t_shell *shell, char **line)
{
    if (shell->is_interactive)
        return interactive(shell, line);
    else
        return non_interactive(shell, line);
}

//func 2
static int should_continue_loop(int status)
{
    if (status == -1)     // error in reading input
        return INPUT_RETRY;
    if (status == 0)      // EOF (Ctrl+D)
        return INPUT_EOF;
    return INPUT_OK;      // valid command
}
//func 3
static int process_command(t_shell *shell, char *line)
{
    if (!line || line[0] == '\0')
        return 0;
    if (handle_command(shell, line) == 0)
    {
		free(line);
		return 0;
    }
    return 1;
    // free(line);
}
//func 4
static void set_signal_status(t_shell *shell)
{
		if (g_signal_status == 130)
		{
			g_signal_status = 0;
			shell->last_exit_status = 130;
		}
}

//func 5
static void  handle_input_status(t_shell *shell, int status, char *cmd)
{
	if (status == INPUT_OK)
		process_command(shell, cmd);
}

//func 6
void main_loop(t_shell *shell)
{
	char *command_line;
    int status;
    int check;
	
    while (1)
    {
		if (shell->is_interactive)
			signals_prompt();
		else
			setup_non_interactive_signals();
		//if (shell->is_interactive)
		//signals_prompt();
		g_signal_status = 0;
        status = get_command_line(shell, &command_line);
		set_signal_status(shell);
        check = should_continue_loop(status);
        if (check == INPUT_EOF)
        {
            printf("exit\n");
            break;
        }
        if (check == INPUT_RETRY)
            continue;
        handle_input_status(shell, status, command_line);
		set_signal_status(shell);
        free(command_line);
    }
}

