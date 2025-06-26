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

typedef struct s_shell
{
	int		argc;
	char	**argv;
	char	**envp;
	int		last_exit_status;
}			t_shell;

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
    t_redirection redir_type;
    char *file;

} t_redir; 

typedef struct s_quote_state
{
	int		in_single;
	int		in_double;
}			t_quote_state;

typedef struct s_command
{
    char **arg; 
    bool is_pipe;
    t_redir **redirection;
    int     redir_count; 
    struct s_command *next;
} t_command;

// main.c 
void greets_minishell(void);

// Parser.c  - 1
t_command    *parser(char *command_line, t_shell *shell);
void    parser2(char **tokens, int *k, t_command **current);
void    add_arg(char *tokens, t_command **current);
void    is_redirection(char **tokens, t_command **current, int *k); // more then 25 lines


// parse_utils.c - 2
char    **tokenizer(char *line);
void    tokenizer2(char **tokens, int *k, int *i, char *line);
void    handle_word(char **tokens, int *k, char *line, int *i, int *wbeg);
void    handle_double_qoute(char **tokens, int *k, char *line, int *i, int *wbeg);
void    handle_single_qoute(char **tokens, int *k, char *line, int *i, int *wbeg);

// parse_utils_2.c  - 3
void    allocate_memory_shell(t_shell **shell);
void    init_shell(t_shell *shell, int argc, char **argv, char **envp);
void	init_cmd(t_command *cmd);
bool    is_redirector(char **tokens, int k);

// env_handler.c - 4
char	**tokens_expanded(char **tokens, t_shell *shell);
char	*expand_variables_in_token(char *input, t_shell *shell);
char	*char_to_str(char c);
char	*handle_sign(char *input, t_shell *shell, int *i);
char	*handle_bash_parameter(t_shell *shell, int *i, char *input);
char	*handle_question_mark(t_shell *shell, int *i);
char	*expand_env_var(char *input, int *i);
char	*get_env(char *value, char **envp);
char	**copying_env(char **old_env);
char	*ft_strjoin_free(char *s1, char *s2);
void	free_env_copy(char **env_copy);
void	free_tokens(char **tokens);


// syntax_check - 5 // lots of more than 25
int     count_words(char *str);
int	    handle_three_and_higher_redir(char *str, int *i);
int     handle_redir_at_beg(char *str, int *i, int *wc);
int	    handle_pipe(char *str, int *i);
void	count_qoute(char *str, int *i, int *wc);
int     handle_syntax(char *str, int *i, int *wc); // more than 25 lines


// utils.c - 6
char    *ft_strncpy(char *des, char *src, int n);
char	*ft_strcpy(char *des, char *src);
int	    ft_strcmp(const char *s1, const char *s2);
t_command   *create_node();

// signals - 7
int     signals_handling(void);
void    handler(int sig);


// // Executing - 8 
// bool is_builtin(char *cmd);
// int exec_builtin(t_command cmd, t_shell shell);
// void execute_cmd(t_command *cmd, t_shell *shell);


// extra help - 9
void print_cmd_list(t_command *head);


// folder execute 

// executor.c
bool is_builtin(char *cmd);
int exec_builtin(t_command *cmd, t_shell *shell);
void execute_cmd(t_command *cmd, t_shell *shell);

#endif