
clear && make re && make clean && clear && echo && valgrind --suppressions=readline_curses.supp --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell
