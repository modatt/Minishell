#include "minishell.h"
// volatile sig_atomic_t g_heredoc_interrupted = 0;
//void	handler(int sig)
//{
//	// (void)sig;
//	/* We don’t need the value itself            */
//	write(1, "\n", 1); /* 1. move the cursor to a new terminal line */
//	if (sig == SIGINT)
//	{
//		rl_on_new_line();       /* 2. tell readline: “we’re on a new line”   */
//		rl_replace_line("", 0); /* 3. clear whatever the user was typing     */
//		rl_redisplay();         /* 4. repaint the prompt + empty input line*/
//		// exit();
//		// free_cmd(free);
//		// free_shell(shell);
//	}
//	// else if (sig == SIGQUIT)
//	return ;
//}

//int	signals_handling(void)
//{
//	struct sigaction	sig;

//	sig.sa_handler = handler;
//	sigemptyset(&sig.sa_mask);
//	sig.sa_flags = SA_RESTART;
//	if (sigaction(SIGINT, &sig, NULL) == -1)
//		return (-1);
//	// Set up SIGQUIT (Ctrl-\) to be ignored
//	sig.sa_handler = SIG_IGN;
//	sigemptyset(&sig.sa_mask);
//	sig.sa_flags = SA_RESTART;
//	if (sigaction(SIGQUIT, &sig, NULL) == -1)
//		return (-1);
//	return (0);
//}

//#include <signal.h>
//#include <unistd.h>
//#include <readline/readline.h>

/* ───────────── Prompt handler ───────────── */
volatile sig_atomic_t g_heredoc_interrupted = 0;




//func 1
void handler_parent(int sig)
{
    (void)sig;
    write(1, "\n", 1);
}
//func 2
void signals_prompt(void)
{
    setup_sig(SIGINT, handler_prompt);
    setup_sig(SIGQUIT, SIG_IGN);
}
//func 3
void signals_execution(void)
{
    setup_sig(SIGINT, SIG_DFL);   // Default → kills child
    setup_sig_exc(SIGQUIT, SIG_DFL);  // Default → sends \Quit (core dump message)
}
//func 4
void handler_parent_quit(int sig)
{
    (void)sig;
    write(1, "Quit (core dumped)\n", 20);
}
//func 5
void handler_heredoc(int sig)
{
    (void)sig;
    g_heredoc_interrupted = 130;
    write(1, "^C", 3);
	close(0);
    signals_prompt();
    //write(STDOUT_FILENO, " Ctrl+C detected in heredoc!\n", 36);
}


//void signals_heredoc(void)
//{
//    setup_sig(SIGINT, handler_heredoc);
//    setup_sig(SIGQUIT, SIG_IGN);
//}
