#include "minishell.h"

// External declaration for the global variable
volatile sig_atomic_t g_signal_status = 0;


void	signals_prompt(void)
{
	//(void)sig;
	//printf("3\n");
	setup_sig(SIGINT, handler_prompt);
	setup_sig(SIGQUIT, SIG_IGN);
}


void	handler_parent(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void	handler_prompt(int sig)
{
	(void)sig;
    g_signal_status = 130;
	//printf("2");
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_sig(int sig, void (*handler)(int))
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(sig, &sa, NULL) == -1)
		{
			perror("signal");
			return ;
		}
}
void	setup_sig_exc(int sig, void (*handler)(int))
{
	struct sigaction	sa;
	g_signal_status = 130;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(sig, &sa, NULL);
}
void	signals_execution(void)
{
	setup_sig_exc(SIGINT, SIG_DFL);
	setup_sig_exc(SIGQUIT, SIG_DFL);
}

void	handler_parent_quit(int sig)
{
	(void)sig;
	write(1, "Quit (core dumped)\n", 20);
}

void close_all_fds()
{
	int fd;
	fd = 0;
	while (fd <1024)
	{
		if(fd == 2)
		{
			fd++;
			continue;
		}
		close(fd);
		fd++;
	}


}

void	signals_heredoc(void)
{
	struct sigaction	sa;

	sa.sa_handler = handler_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
void	handler_heredoc(int sig)
{
	//(void)sig;
	//printf("\n");
	write(1, "\n", 1);
	close(0);
	//exit(0);
	//close_all_fds();
	if(sig == SIGINT)
		g_signal_status = 130;
	//signals_prompt();
}

// sig handler
// Handler for SIGINT in the main prompt
void handle_sigint_prompt(int sig)
{
    (void)sig; // Suppress unused parameter warning
    g_signal_status = SIGINT;
    //printf("4\n");
    // Provide immediate feedback for interactive mode
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();       // Tell readline we're on a new line
    rl_replace_line("", 0); // Clear the current line buffer
    rl_redisplay();         // Redisplay prompt and empty line
}

// Handler for SIGQUIT in the main prompt
//void handle_sigquit_prompt(int sig)
//{
//    printf("5\n");
//    (void)sig; // Suppress unused parameter warning
//    g_signal_status = SIGQUIT;
//    // For interactive shell, SIGQUIT is usually ignored at the prompt level.
//    // If you enable it, you might print "Quit: 3" message here.
//    // For now, it just sets the status for the parent to check.
//}

// Handler for SIGINT during heredoc input
//void handle_sigint_heredoc(int sig)
//{
//    printf("6\n");
//    (void)sig; // Suppress unused parameter warning
//    g_signal_status = SIGINT; // Set status for heredoc interruption
//    // This is crucial for heredoc to stop blocking on read.
//    // Sending a newline might help break out of `read` in some cases.
//    // Just exit heredoc process (child)
//    write(STDOUT_FILENO, "\n", 1);
//    exit(130);
//    // Depending on your heredoc implementation (e.g., if it's a child process
//    // exclusively for heredoc), you might `exit(130)` here directly in the child.
//    // For a single process loop, setting g_signal_status is sufficient.
//}
//void setup_heredoc_signals(void)
//{
//    struct sigaction sa;

//    sa.sa_handler = handle_sigint_heredoc;
//    sigemptyset(&sa.sa_mask);
//    sa.sa_flags = 0;

//    sigaction(SIGINT, &sa, NULL);
//    signal(SIGQUIT, SIG_IGN);
//}

// sig setup 
// interactive

// Sets up signals for the interactive prompt mode (minishell's main loop).
// SIGINT will update g_signal_status. SIGQUIT will be ignored to prevent exiting.
int setup_interactive_signals(void)
{
    printf("7\n");
    struct sigaction sa_int;
    struct sigaction sa_quit;

    // Setup SIGINT handler
    sa_int.sa_handler = handle_sigint_prompt;
    sigemptyset(&sa_int.sa_mask); // Clear the mask; no signals blocked during handler execution
    sa_int.sa_flags = SA_RESTART; // Restart interrupted system calls like `read` or `readline`
    if (sigaction(SIGINT, &sa_int, NULL) == -1)
    {
        perror("minishell: sigaction SIGINT (interactive)");
        return (-1);
    }
    sa_quit.sa_handler = SIG_IGN; // Ignore SIGQUIT
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0; // No special flags
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
    {
        perror("minishell: sigaction SIGQUIT (interactive)");
        return (-1);
    }
    return (0);
}

// non interactive
// Sets up signals for non-interactive mode.
// SIGINT and SIGQUIT are ignored by the shell itself to not interfere with scripts.

int setup_non_interactive_signals(void)
{
    struct sigaction sa;

    printf("8\n");
    sa.sa_handler = SIG_DFL; // default handling
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("minishell: sigaction SIGINT (non-interactive)");
        return (-1);
    }
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        perror("minishell: sigaction SIGQUIT (non-interactive)");
        return (-1);
    }
    return (0);
}


// Sets up signals for child processes (executing commands).
// Children should revert to default signal handling or specific handling for builtins.
// This allows them to be terminated by SIGINT/SIGQUIT.

//int setup_child_signals(void)
//{
//    struct sigaction sa_int;
//    struct sigaction sa_quit;

//    // Set SIGINT to default action (terminate process)
//    sa_int.sa_handler = SIG_DFL;
//    sigemptyset(&sa_int.sa_mask);
//    sa_int.sa_flags = 0;
//    if (sigaction(SIGINT, &sa_int, NULL) == -1)
//    {
//        perror("minishell: sigaction SIGINT (child)");
//        return (-1);
//    }

//    // Set SIGQUIT to default action (terminate process and dump core)
//    sa_quit.sa_handler = SIG_DFL;
//    sigemptyset(&sa_quit.sa_mask);
//    sa_quit.sa_flags = 0;
//    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
//    {
//        perror("minishell: sigaction SIGQUIT (child)");
//        return (-1);
//    }
//    return (0);
//}
// Blocks SIGINT and SIGQUIT temporarily in the parent before forking.
// This prevents signals from interrupting the fork/exec sequence.
// 'old_mask' will store the process's signal mask before blocking.
// Returns 0 on success, -1 on error.
//int block_fork_signals(sigset_t *old_mask)
//{
//    sigset_t new_mask;

//    sigemptyset(&new_mask);
//    sigaddset(&new_mask, SIGINT);
//    sigaddset(&new_mask, SIGQUIT);

//    // Add SIGINT and SIGQUIT to the current signal mask,
//    // storing the old mask in `old_mask` so it can be restored.
//    if (sigprocmask(SIG_BLOCK, &new_mask, old_mask) == -1)
//    {
//        perror("minishell: sigprocmask BLOCK");
//        return (-1);
//    }
//    return (0);
//}

//int unblock_fork_signals(sigset_t *old_mask)
//{
//    // Set the process's signal mask back to `old_mask`
//    if (sigprocmask(SIG_SETMASK, old_mask, NULL) == -1)
//    {
//        perror("minishell: sigprocmask UNBLOCK");
//        return (-1);
//    }
//    return (0);
//}

//int signals_non_interactive(void) { return 0; }
