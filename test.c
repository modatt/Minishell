#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>

volatile sig_atomic_t g_heredoc_interrupted = 0;

void handler_heredoc(int sig)
{
    (void)sig;
    g_heredoc_interrupted = 1;
    write(1, "\n[DEBUG] Ctrl+C detected in heredoc!\n", 38);
}

void signals_heredoc(void)
{
    struct sigaction sa;
    sa.sa_handler = handler_heredoc;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // no SA_RESTART
    sigaction(SIGINT, &sa, NULL);
}

int main(void)
{
    rl_catch_signals = 0;  // <-- MUST BE HERE BEFORE readline()

    signals_heredoc();

    while (1) {
        char *line = readline("> ");
        if (!line) {
            if (g_heredoc_interrupted) {
                printf("Aborting heredoc\n");
                return 0;
            } else {
                printf("EOF (Ctrl+D)\n");
                break;
            }
        }
        printf("Got line: %s\n", line);
        free(line);
    }
}
