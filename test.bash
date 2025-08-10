
clear && make re && make clean && clear && echo && valgrind --suppressions=readline_curses.supp --leak-check=full ./minishell
# clear && make re && make clean && clear && echo && valgrind --suppressions=readline_curses.supp --leak-check=full --trace-children=yes --show-leak-kinds=all --track-fds=yes ./minishell
