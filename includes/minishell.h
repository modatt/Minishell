/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: modat <modat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:46:25 by modat             #+#    #+#             */
/*   Updated: 2025/08/12 08:12:14 by modat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
#  define READLINE_BUFFER_SIZE 1024 // Define a buffer size for reading chunks
#  define INPUT_EOF 0               // Exit the main loop
#  define INPUT_OK 1                // Proceed with parsing and execution
#  define INPUT_RETRY 2             // Continue loop without parsing
#  define PATH_MAX 4096
# endif

# define WHITE "\x1B[37m"
# define BLUE "\x1B[34m"
# define RED "\x1B[31m"
# define GRN "\x1B[32m"
# define RESET "\x1B[0m"
# define BEIGE "\x1B[38;5;230m"
# define PEACH "\x1B[38;5;217m"

// grouping the operators that separate the command based on
extern volatile sig_atomic_t	g_signal_status;

typedef enum e_redirection_type
{
	REDIR_NONE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}								t_redirection;

typedef struct s_read_buffer
{
	char						*buffer;
	int							buffer_pos;
	int							bytes_in_buffer;
}								t_read_buffer;

typedef struct s_env_var
{
	char						*name;
	char						*value;
	bool						exported;
	struct s_env_var			*next;
}								t_env_var;

typedef struct s_shell
{
	int							argc;
	char						**argv;
	int							last_exit_status;
	char						**envp;
	t_env_var					*env_list;
	int							is_interactive;
}								t_shell;

typedef enum e_buildins
{
	CD,
	ECHO,
	ENV,
	PWD,
	EXPORT,
	EXIT,
	UNSET
}								t_buildins;

typedef struct s_redir
{
	t_redirection				redir_type;
	char						*file;

}								t_redir;

typedef struct s_quote_state
{
	int							in_single;
	int							in_double;
}								t_quote_state;

typedef struct s_quote_data
{
	int							quoted_start;
	int							quoted_len;
	int							continuation_start;
	int							continuation_len;
}								t_quote_data;

typedef struct s_token_data
{
	char						**tokens;
	int							*k;
	char						*line;
	int							*i;
	int							*wbeg;
}								t_token_data;

typedef struct s_command
{
	char						**arg;
	bool						is_pipe;
	t_redir						**redirection;
	int							redir_count;
	struct s_command			*next;
}								t_command;

typedef struct s_pipeline_data
{
	pid_t						*child_pids;
	int							*pid_count;
	int							*prev_pipe_rd_fd;
}								t_pipeline_data;

// main.c
void							greets_minishell(void);
bool							is_redirector(char **tokens, int k);
// mo9eba change int to bool bool to int

int								ft_isspace(char s);
// Parser.c  - 1
t_command						*parser(char *command_line, t_shell *shell);
void							parser2(char **tokens, int *k,
									t_command **current);
void							add_arg(char *tokens, t_command **current);
void							is_redirection(char **tokens,
									t_command **current, int *k);

// parser_utils.c - helper functions for parser
void							copy_existing_args(char **new_arg,
									char **old_arg, int count);
int								init_redirection_array(t_command **current);
void							set_redir_type(t_redir *redir, char *token);
int								process_redir_file(t_redir *redir,
									char **tokens, int *k);
int								handle_current_setup(t_command **current,
									t_command **cmd_list, char **new_tokens);

// parse_utils.c - 2
char							**tokenizer(char *line, t_shell *shell);
void							tokenizer2(char **tokens, int *k, int *i,
									char *line);
void							handle_word_enhanced(t_token_data *data);
void							handle_word(t_token_data *data);
void							handle_double_qoute(t_token_data *data);
void							handle_single_qoute(t_token_data *data);
int								handle_less(char *str, int *i);
int								handle_greater(char *str, int *i);

// parse_utils_4.c - helper functions
void							process_double_quotes(char *line, int *i,
									char *result, int *result_len);
void							process_single_quotes(char *line, int *i,
									char *result, int *result_len);
void							finalize_token(char **tokens, int *k,
									char *result, int result_len);
int								parse_quoted_content(char *line, int *i,
									int *quoted_start);
int								parse_continuation(char *line, int *i,
									int *continuation_start);

// parse_utils_5.c - helper functions
void							build_double_quote_token(char **tokens, int *k,
									char *line, t_quote_data *data);
int								parse_single_quoted_content(char *line, int *i,
									int *quoted_start);
int								parse_single_continuation(char *line, int *i,
									int *continuation_start);
void							build_single_quote_token(char **tokens, int *k,
									char *line, t_quote_data *data);

// parse_utils_2.c  - 3
void							allocate_memory_shell(t_shell **shell);
int								init_shell(t_shell *shell, int argc,
									char **argv, char **envp);
void							init_command(t_command *cmd);

// env_handler.c - 4
char							**tokens_expanded(char **tokens,
									t_shell *shell);
char							*expand_variables_in_token(char *input,
									t_shell *shell);
char							*char_to_str(char c);
char							*handle_sign(char *input, t_shell *shell,
									int *i);
char							*handle_bash_parameter(t_shell *shell, int *i,
									char *input);
char							*handle_question_mark(t_shell *shell, int *i);
char							*expand_env_var(char *input, int *i);
char							*get_envp(char **envp, char *value);
char							*get_env(char *key, t_env_var *env_list);
char							**copying_env(char **old_env);
char							*ft_strjoin_free(char *s1, char *s2);
void							free_env_copy(char **env_copy);
void							free_tokens(char **tokens);
char							*get_envp(char **envp, char *value);

// syntax_check - 5
int								count_words(char *str);
int								handle_three_and_higher_redir(char *str,
									int *i);
int								handle_redir_at_beg(char *str, int *i, int *wc);
int								handle_pipe(char *str, int *i);
void							count_qoute(char *str, int *i, int *wc);
int								handle_syntax(char *str, int *i, int *wc);
int								handle_start(char *s, int *i, int *wc);

// syntax_check_utils_3.c
int								handle_start(char *s, int *i, int *wc);
// utils.c - 6
char							*ft_strncpy(char *des, char *src, int n);
char							*ft_strcpy(char *des, char *src);
int								ft_strcmp(const char *s1, const char *s2);
t_command						*create_node(void);


// folder execute

// executor.c
void							execute_cmd(t_command *cmd, t_shell *shell);

// redirections.c

void							setup_redirection_fds(t_command *cmd);
void							maybe_preprocess_heredocs(t_command *cmd);
void							preprocess_heredocs(t_command *cmd);
int								write_heredoc_to_file(char *tmpfile,
									char *delimiter);
void							run_with_redirection(t_command *cmd,
									t_shell *shell);
// execu_buildins_1.c
bool							is_builtin(char *cmd);
int								exec_builtin(t_command *cmd, t_shell *shell);

// exec_buildins_2.c
void							builtin_echo(t_command *cmd, t_shell *shell);
void							builtin_pwd(t_command *cmd);
void							builtin_env(t_shell *shell);
int								handle_export_var_cd(char *name, char *value,
									t_shell *shell, int status);
void							xx(t_command *cmd);
// exce_extranal.c
void							exec_external(t_command *cmd, t_shell *shell);
// get_next_line
char							*get_next_line(int fd);
// exce_external_utils.c
void							print_command_not_found(t_command *cmd,
									t_shell *shell);
char							*join_path_cmd(char *dir, char *cmd);
char							**get_path_dirs(t_shell *shell);
char							*find_cmd_in_path(char *arg, t_shell *shell);
char							*get_resolved_path(t_command *cmd,
									t_shell *shell);

// exit
// and static one
long							ft_atol(const char *nptr);
void							clean_exit(t_shell *shell, long exit_code,
									t_command *cmd);
void							builtin_exit(t_command *cmd, t_shell *shell);

// exit_utils.c
int								is_numeric(char *str);
int								args_count(char **str);

// init_env_list.c
void							init_env_list(t_shell *shell, char **envp);
void							add_env_var(t_env_var **list, t_env_var *new);
t_env_var						*new_env_var(char *name, char *value);
t_env_var						*find_var(char *name, t_env_var *env_list);

// export.c
int								builtin_export(t_command *cmd, t_shell *shell);
int								handle_export_var(char *name, char *value,
									t_shell *shell, int status);
int								update_or_add_var(char *name, char *value,
									t_shell *shell);
void							add_var_to_list(t_env_var **env_list,
									t_env_var *new_var);
int								handle_export_var(char *name, char *value,
									t_shell *shell, int status);
// export_utils.c
void							print_exported_list(t_shell *shell);
void							parse_arg_var(char *arg, char **name,
									char **value);
bool							is_name_valid(const char *name);
void							print_export_error(char *name);
void							name_invalid(char *name, char *value);

// export_utils_2.c
t_env_var						*create_var(char *name, char *value,
									bool booling);
int								init_var_fields(t_env_var *var, char *name,
									char *value, bool exported);
void							update_var_value(t_env_var *exist,
									char *new_value);
void							free_name_value(char *name, char *value);

// unset.c
int								builtin_unset(t_command *cmd, t_shell *shell);
void							remove_var(t_env_var **env_list,
									const char *name);

// cd.c
void							builtin_cd(t_command *cmd, t_shell *shell);
int								handle_cd_errors(t_shell *shell, char *old_pwd,
									char *target);
int								cd_update_pwd(t_shell *shell, char *old_pwd);
// free.c
void							free_cmd(t_command *cmd);
void							free_redir(t_command *cmd);
void							free_shell(t_shell *shell);
void							free_env_list(t_shell *shell);

// pipeline.c & pipeline_utils files
void							execute_pipeline(t_command *cmd_list,
									t_shell *shell);
pid_t							*allocate_pid_array(int cmd_count);
int								count_commands(t_command *cmd_list);
void							handle_pipe_error(int prev_pipe_read_fd,
									pid_t *child_pids);
void							handle_fork_error(t_command *current_cmd,
									int pipe_fds[2], int prev_pipe_read_fd,
									pid_t *child_pids);
void							setup_input_redirection(int prev_pipe_read_fd);
void							setup_output_pipe(t_command *current_cmd,
									int pipe_fds[2]);
int								create_pipe_and_fork(t_command *current_cmd,
									t_pipeline_data *data, int pipe_fds[2]);
void							open_output_file(t_command *current_cmd, int i,
									int *fd_out);
int								setup_pipeline_execution(t_command *current_cmd,
									t_shell *shell, t_pipeline_data *data,
									int pipe_fds[2]);
void							init_pipeline_data(t_command *cmd_list,
									t_pipeline_data *data, int *prev_pipe_rd_fd,
									int *pid_count);
void							execute_command(t_command *current_cmd,
									t_shell *shell);
void							wait_for_children(int pid_count,
									t_shell *shell);
void							handle_input_file_redir(t_command *current_cmd);
void							handle_output_file_redi(t_command *current_cmd);

void							update_shlvl(t_shell *shell);
void							update_envp_array(t_shell *shell,
									const char *name, const char *value);

// non_interactive.c
char							*readline_non_interactive(int fd);
int								signals_non_interactive(void);
void							reset_child_signals(void);
int								non_interactive(t_shell *shell,
									char **command_line);
int								interactive(t_shell *shell,
									char **command_line);
int								handle_command(t_shell *shell, char *line);
void							main_loop(t_shell *shell);
void							parent_handle_pipe(pid_t pid, t_shell *shell);
// signals_interactive.c
void							signals_prompt(void);
void							handler_prompt(int sig);
void							handler_parent(int sig);
void							signals_heredoc(void);
void							setup_sig(int sig, void (*handler)(int));
void							signals_execution(void);
void							setup_sig_exc(int sig, void (*handler)(int));
void							handler_parent_quit(int sig);
void							handler_heredoc(int sig);
void							setup_redirection_fds(t_command *cmd);

void							handle_sigint_prompt(int sig);
void							handle_sigquit_prompt(int sig);
void							handle_sigint_heredoc(int sig);
void							setup_heredoc_signals(void);
int								setup_interactive_signals(void);
int								setup_non_interactive_signals(void);
int								setup_child_signals(void);
int								block_fork_signals(sigset_t *old_mask);
int								unblock_fork_signals(sigset_t *old_mask);
void							handle_input_status(t_shell *shell, int status,
									char **cmd);
int								init_read_buffer(t_read_buffer *rb);
int								fill_buffer(int fd, t_read_buffer *rb);
t_shell							*create_shell(int argc, char **argv,
									char **envp);
int								process_command(t_shell *shell, char **line);
void							handle_signal_pipe(void);
void							preprocess_heredocs_pipe(t_command *cmd);
// redirections utils
int								get_redirection_fd(t_redir *redir);
int								heredoc_process_line(char *line,
									char *delimiter, int fd, char *tmpfile);
int								heredoc_interrupted(char *line, int fd);
// redirections 1
int								handle_heredoc_eof(char *line, int fd,
									char *tmpfile, char *delimiter);
#endif
