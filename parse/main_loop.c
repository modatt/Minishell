#include "minishell.h"
#define INPUT_EOF     0  // Exit the main loop
#define INPUT_OK      1  // Proceed with parsing and execution
#define INPUT_RETRY   2  // Continue loop without parsing

static int handle_signals(t_shell *shell)
{
    if (g_signal_status == SIGINT)
    {
        if (shell->is_interactive)
        {
            write(STDOUT_FILENO, "\n", 1);
            rl_replace_line("", 0);
            rl_on_new_line();
            rl_redisplay();
            shell->last_exit_status = 130;
            g_signal_status = 0; // Reset signal status after handling
            return (1);
        }
    }
    else if (g_signal_status == SIGQUIT)
    {
        if (shell->is_interactive)
        {
            rl_on_new_line();
            rl_redisplay();
            shell->last_exit_status = 131;
            g_signal_status = 0; // Reset signal status after handling
            return (1);
        }
    }
    return 0; // no signal
}

static int get_command_line(t_shell *shell, char **line)
{
    if (shell->is_interactive)
        return interactive(shell, line);
    else
        return non_interactive(shell, line);
}

static int should_continue_loop(int status)
{
    if (status == -1)     // error in reading input
        return INPUT_RETRY;
    if (status == 0)      // EOF (Ctrl+D)
        return INPUT_EOF;
    return INPUT_OK;      // valid command
}

static void process_command(t_shell *shell, char *line)
{
    if (!line || line[0] == '\0') {
        // Empty input, nothing to process or free
        return;
    }
    if (!handle_command(shell, line))
    {
        free(line);
        return;
    }
    free(line);
}

void main_loop(t_shell *shell)
{
    char *command_line;
    int status;
    int check;

    while (1)
    {
        shell->heredoc_interrupted = 0;
        if (shell->is_interactive)
            setup_interactive_signals();
        else
            setup_non_interactive_signals();
        if (handle_signals(shell))
            continue;
        status = get_command_line(shell, &command_line);
        check = should_continue_loop(status);
        if (check == INPUT_EOF)
        {
            printf("exit\n");
            break;
        }
        if (check == INPUT_RETRY)
            continue;
        if (check == INPUT_OK)
            process_command(shell, command_line);
    }
}
