#include "minishell.h"

void    display_history(void);

void    handler(int sig)
{
    // (void)sig;                 /* We don’t need the value itself            */
    write(1, "\n", 1);         /* 1. move the cursor to a new terminal line */
    if (sig == SIGINT )
    {
        rl_on_new_line();          /* 2. tell readline: “we’re on a new line”   */
        rl_replace_line("", 0);    /* 3. clear whatever the user was typing     */
        rl_redisplay();            /* 4. repaint the prompt + empty input line*/
    }
    // else if (sig == SIGQUIT)
    return ;
}

int     signals_handling(void)
{
    struct sigaction sig;
    sig.sa_handler = handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sig, NULL) == -1)
        return -1;
      // Set up SIGQUIT (Ctrl-\) to be ignored
    sig.sa_handler = SIG_IGN;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;
    if (sigaction(SIGQUIT, &sig, NULL) == -1)
        return -1;
    return 0;
}

int main(void)
{
    char *command_line;
    
    if (signals_handling() == -1)
        return -1;

    while (1)
    {
        command_line = readline("minishell$ ");
        if (command_line == NULL)
            exit (-1);
    
        if (*command_line) //* if the first character not '\0' then the string isn't empty
            add_history(command_line);
        if (strcmp(command_line, "clear") == 0)
        {
            rl_clear_history(); // to wipe out all previously added commands from the in-memory history list
            printf("History cleaned\n");
        }
        if (strcmp(command_line, "history") == 0)
            display_history();
    }
    free(command_line);
    return 0;
}


void    display_history(void)
{
    int i = 0;
    HIST_ENTRY **the_hist = history_list();
    if (the_hist == NULL)
    {
        printf("No history.\n");
        return ;
    }
    while (the_hist[i])
    {
        printf("%d: %s\n", i + history_base, the_hist[i]->line);
        i++;
    }
}