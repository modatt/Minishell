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

typedef struct s_quote_state
{
	int		in_single;
	int		in_double;
}			t_quote_state;

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

typedef struct s_command
{
    // char *cmd; 
    char **arg; 
    bool is_pipe;
    t_redir **redirection;
    int     redir_count; 
    struct s_command *next;
} t_command;

// opening 
void greets_minishell(void);

//Parsing 
t_command    *parser(char *command_line, t_shell *shell);
char    **tokenizer(char *line);
int     count_words(char *str);
char    *ft_strncpy(char *des, char *src, int n);
void    init_cmd(t_command *cmd);
void    add_arg(char *tokens, t_command **current);
bool    expand_wildcard(char *str);
bool    is_redirector(char **tokens, int k);
void    is_redirection(char **tokens, t_command **current, int *k);
void    parser2(char **tokens, int *k, t_command **current);
t_command   *create_node();
void    tokenizer2(char **tokens, int *k, int *i, char *line);
void    handle_double_qoute(char **tokens, int *k, char *line, int *i, int *wbeg);
void    handle_single_qoute(char **tokens, int *k, char *line, int *i, int *wbeg);
void    handle_word(char **tokens, int *k, char *line, int *i, int *wbeg);
int    handle_three_and_higher_redir(char *str, int *i);
int     handle_redir_at_beg(char *str, int *i, int *wc);
void    init_shell(t_shell *shell, int argc, char **argv, char **envp);
// char    **new_parser(char *command_line);

int	    handle_pipe(char *str, int *i);
int	handle_three_and_higher_redir(char *str, int *i);
int     handle_redir_at_beg(char *str, int *i, int *wc);
void	count_qoute(char *str, int *i, int *wc);
int     handle_syntax(char *str, int *i, int *wc);

// void    free_tokens(char **tokens);

// Executing
bool is_builtin(char *cmd);
int exec_builtin(t_command cmd, t_shell shell);
void execute_cmd(t_command *cmd, t_shell *shell);


// env_handler
char **copying_env(char **old_env);
char *get_env(char *value, char **envp);
char	*expand_env_var(char *input, int *i);
char *handle_question_mark(t_shell *shell, int *i);
char	*handle_sign(char *input, t_shell *shell, int *i);
char	*char_to_str(char c);
void free_env_copy(char **env_copy);
char	*handle_single_quote(char *input, int *i);
char	*expand_variables_in_token(char *input, t_shell *shell);
void	free_tokens(char **tokens);
char	**tokens_expanded(char **tokens, t_shell *shell);

// signals
int     signals_handling(void);
void    handler(int sig);

// utils 
int	ft_strcmp(const char *s1, const char *s2);
char    *ft_strcpy(char *des, char *src);
#endif