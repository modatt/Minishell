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
#include <stdbool.h>

// grouping the operators that separate the command based on 

typedef enum s_redirections
{
    NONE,
    DOUBLE_OUTFILE,
    DOUBLE_INFILE,
    SINGLE_OUTFILE,
    SIGNLE_INFILE
} t_redirections; 

typedef enum s_buildin
{
    CD,
    ECHO,
    ENV, 
    PWD,
    EXPORT,
    EXIT,
    UNSET
} t_buildin;


typedef struct s_command
{
    char **arg; 
    bool is_pipe;
    t_redirections redirection;
    char *file;
    int   index;
    int is_wildcard;
    struct s_command *next;
} t_command;




// Parsing 
void    parser(char *command_line);
char    **tokenizer(char *line);
int     count_words(char *str);
char    *ft_strncpy(char *des, char *src, int n);
int     detect_operator(char *token, t_command *command);
void    normal(char *tokens, char *line, int i, int wbeg, int k);
void    qoute_handler(char **str, int i, int wc);


// signals
int     signals_handling(void);

// int     strcmp_whitespaces_handle(char *s1, char *s2);
void    parser(char *str);
// void    display_history(void);
// void    printdoup(char **str);
void    handler(int sig);
#endif