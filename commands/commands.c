#include "philosophers.h"

void    commands(char *str, int n)
{
    if (ft_strncmp(str, n) == "echo")
    {
	    ft_print();
    }
    if (dollar_sign == 1)
    {
	    //handle expansion of $
	}
    if (minus_n == 1)
    {
	    //handle -n
    }
    if (exit_command() == 1)
    {
        rl_on_new_line();          /* 2. tell readline: “we’re on a new line”   */
        rl_replace_line("", 0);    /* 3. clear whatever the user was typing     */
        rl_redisplay();            /* 4. repaint the prompt + empty input line*/

        // exit & free allocated memory & distroy connection might be built 
        exit(1);

        /*
        Ends the current process (your shell session)

Cleans up resources:

Closes open files

Frees memory

Notifies the OS that the process has ended
        */
    }
}

