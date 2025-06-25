#include "minishell.h"

void	handler(int sig)
{
	// (void)sig;
	/* We don’t need the value itself            */
	write(1, "\n", 1); /* 1. move the cursor to a new terminal line */
	if (sig == SIGINT)
	{
		rl_on_new_line();       /* 2. tell readline: “we’re on a new line”   */
		rl_replace_line("", 0); /* 3. clear whatever the user was typing     */
		rl_redisplay();         /* 4. repaint the prompt + empty input line*/
	}
	// else if (sig == SIGQUIT)
	return ;
}

int	signals_handling(void)
{
	struct sigaction	sig;

	sig.sa_handler = handler;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sig, NULL) == -1)
		return (-1);
	// Set up SIGQUIT (Ctrl-\) to be ignored
	sig.sa_handler = SIG_IGN;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = SA_RESTART;
	if (sigaction(SIGQUIT, &sig, NULL) == -1)
		return (-1);
	return (0);
}