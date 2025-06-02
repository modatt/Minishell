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


    #include <stdio.h>
    #define WHITE "\x1B[37m"
    #define BLUE "\x1B[34m"
    #define RED "\x1B[31m"
    #define GRN "\x1B[32m"
    #define RESET "\x1B[0m"
    #define BEIGE "\x1B[38;5;230m"
    #define PEACH "\x1B[38;5;217m"

// grouping the operators that separate the command based on 

typedef enum e_redirection_type
{
    REDIR_NONE,
    REDIR_INPUT,       // <
    REDIR_OUTPUT,      // >
    REDIR_APPEND,      // >>
    REDIR_HEREDOC      // <<
} t_redirection;


typedef enum e_buildins
{
    CD,
    ECHO,
    ENV, 
    PWD,
    EXPORT,
    EXIT,
    UNSET
} t_buildins;

typedef struct s_redir
{
    t_redirection redir;
    char *file;

} t_redir; 

typedef struct s_command
{
    char **arg; 
    bool is_pipe;
    // t_redir **redirction;
    t_redirection redirection;
    char *file;
    bool is_wildcard;
    struct s_command *next;
} t_command;


// opening 
void greets_minishell(void);

// Parsing 
// t_command    *parser(char *command_line);
// char    **tokenizer(char *line);
// int     count_words(char *str);
// char    *ft_strncpy(char *des, char *src, int n);
// void    init_cmd(t_command *cmd);
// void    add_arg(char *tokens, t_command **current);
// bool    expand_wildcard(char *str);
// bool    is_redirector(char **tokens, int k);
// void    is_redirection(char **tokens, t_command **current, int *k);
// void    parser2(char **tokens, int *k, t_command **current);
// t_command   *create_node();
char    **new_parser(char *command_line);

// Executing
//void    executor(t_command cmd_list);


// signals
int     signals_handling(void);

// int     strcmp_whitespaces_handle(char *s1, char *s2);

// void    display_history(void);
// void    printdoup(char **str);
void    handler(int sig);

// utils 
int	ft_strcmp(const char *s1, const char *s2);
bool    expand_waildcard(char *str);
char    *ft_strcpy(char *des, char *src);
#endif