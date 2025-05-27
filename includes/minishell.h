#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <signal.h>

// grouping the operators that separate the command based on 
typedef enum e_operator
{
    NONE,
    PIPE,
    AND,
    OR
} t_operator;

typedef struct s_command
{
    char *command;
    char *arg;
    t_operator operator;
    // char *redircties;
    // int is_pipe;
    // int is_directory;
    // int is_wildcard;
    // int is_and;
    struct s_command *next;
        
} t_command;

// Parsing 
void    parser(char *command_line);
char    **tokenizer(char *line);
int     count_words(char *str);
char    *ft_strncpy(char *des, char *src, int n);

// signals
int     signals_handling(void);

int     strcmp_whitespaces_handle(char *s1, char *s2);
void    parser(char *str);
void    display_history(void);
void    printdoup(char **str);
void    handler(int sig);
#endif