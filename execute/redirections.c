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
static void	write_heredoc_to_file(t_redir *redir, char *tmpfile)
{
	int		fd;
	char	*line;

	fd = open(tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(tmpfile);
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
}
//func --- 4
void	preprocess_heredocs(t_command *cmd)
{
	int		i;
	t_redir	*redir;
	char	*tmpfile;
	char	*idx_str;

	i = 0;
	while (i < cmd->redir_count)
	{
		redir = cmd->redirection[i];
		if (redir->redir_type == REDIR_HEREDOC)
		{
			idx_str = ft_itoa(i);
			tmpfile = ft_strjoin("/tmp/minishell_heredoc_", idx_str);
			free(idx_str);

			write_heredoc_to_file(redir, tmpfile);

			free(redir->file);
			redir->file = tmpfile;
			redir->redir_type = REDIR_INPUT;
		}
		i++;
	}
}



