#include "minishell.h"

//func --- 1
void setup_redirection_fds(t_command *cmd)
{
    int i = 0;
    int fd;
	t_redir *redir;

    while (i < cmd->redir_count)
    {
        fd = -1;
        redir = cmd->redirection[i];  // because t_redir**
        if (redir->redir_type == REDIR_INPUT)//< 
            fd = open(redir->file, O_RDONLY);
        else if (redir->redir_type == REDIR_OUTPUT)// > 
            fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (redir->redir_type == REDIR_APPEND) // >> write after it 
            fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
            exit(1);
        if (redir->redir_type == REDIR_INPUT)
            dup2(fd, STDIN_FILENO);//0
        else
            dup2(fd, STDOUT_FILENO);// 1
        close(fd);
        i++;
    }
}

//func --- 2
void maybe_preprocess_heredocs(t_command *cmd)
{
    if (cmd && cmd->redir_count > 0)
        preprocess_heredocs(cmd);
}
//func --- 3
void write_heredoc_to_file(t_redir *redir, char *tmpfile, char *delimiter)
{
    int     fd;
    char    *line;

    (void)redir;
	rl_catch_signals = 0;
    signals_heredoc();
    fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd < 0)
    {
        perror("minishell: heredoc open failed");
        free(tmpfile);
        signals_prompt();
        exit(1);
    }
    while (1)
    {
		line = readline("> ");
        if (!line)
        {
            if (g_heredoc_interrupted)
            {
                close(fd);
                unlink(tmpfile);  // delete temp file
                //free(tmpfile);
                g_heredoc_interrupted = 130;
                signals_prompt();
                return ;
            }
            else if (!line && g_heredoc_interrupted != 130)
            {
                write(STDERR_FILENO,
                      "minishell: warning: here-document delimited by end-of-file (wanted `",
                      72);
                write(STDERR_FILENO, delimiter, ft_strlen(delimiter));
                write(STDERR_FILENO, "`)\n", 3);
                break;
            }
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
    	//signals_heredoc();
        free(line);
    }
	//printf("are we here??\n");
    close(fd);
    signals_prompt();
}




//func --- 4
void preprocess_heredocs(t_command *cmd)
{
    int     i;
    t_redir *redir;
    char    *tmpfile;
    char    *idx_str;
    char    *delimiter;

    i = 0;
    while (i < cmd->redir_count)
    {
        redir = cmd->redirection[i];
        if (redir->redir_type == REDIR_HEREDOC)
        {
            delimiter = ft_strdup(redir->file);
            idx_str = ft_itoa(i);
            tmpfile = ft_strjoin("/tmp/minishell_heredoc_", idx_str);
            free(idx_str);

            write_heredoc_to_file(redir, tmpfile, delimiter);

            // ✅ If Ctrl+C → abort everything
            if (g_heredoc_interrupted)
            {
                free(tmpfile);
                free(delimiter);
                return; // stop further heredoc processing
            }

            free(redir->file);
            free(delimiter);
            redir->file = tmpfile;
            redir->redir_type = REDIR_INPUT;
        }
        i++;
    }
}




