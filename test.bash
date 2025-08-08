
clear && make re && make clean && clear && echo && valgrind --suppressions=readline_curses.supp --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes ./minishell
