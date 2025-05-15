#include "minishell.h"

#include <ctype.h>
#include <string.h>

int strcmp_whitespaces_handle(char *s1, char *s2)
{
    int i = 0;

    // Skip leading whitespace in s1
    while (s1[i] == ' ' || s1[i] == '\t')
        i++;

    int start = i;
    // Move through the first word (alphabetic characters only)
    while (isalpha(s1[i]))
        i++;
    // int end = i;
    while (s1[i] == ' ' || s1[i] == '\t')
        i++;
    if (ft_strncmp(&s1[start], s2, 4) == 0 && s1[i] == '\0')
        return 0;
    else 
        {
            printf("Usage: clear [options]\nOptions:\n");
            printf("  -T TERM     use this instead of $TERM\n  -V          print curses-version\n  -x          do not try to clear scrollback\n");
            return -1;
        }
    return (-1);

}


