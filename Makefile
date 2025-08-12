.PHONY: all clean fclean re

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -g

# Directories
PARSE_DIR = parse
EXECUTE_DIR = execute

INC_DIR = includes
LIBFT_DIR = libft

# Libraries
READLINE = -lreadline
LIBFT = $(LIBFT_DIR)/libft.a

# Project name
NAME = minishell

# Source file base names
PARSE_FILES = minishell greets parser parser_utils parse_utils parse_utils_2 \
	env_handler syntax_check env_handler_utils env_handler_utils_2 \
	syntax_check_utils syntax_check_utils_1 syntax_check_utils_2 parse_utils_1 parse_utils_3 parse_utils_4 parse_utils_5 \
	shlvl non_interative  signals_interactive main_loop signals_setup utils_1 utils_2 syntax_check_utils_3
	


EXECUTE_FILES = executor exec_buildins_1  exec_buildins_2 exit exit_utils_1  init_env_list \
	export export_utils_1 export_utils_2 unset free cd exce_external exce_external_utils \
	pipeline pipeline_utils pipeline_utils_2 pipeline_utils_3 redirections_1 redirections_2 redirections_utils 

# Source file paths
SRCS = $(addsuffix .c, $(addprefix $(PARSE_DIR)/, $(PARSE_FILES))) \
	   $(addsuffix .c, $(addprefix $(EXECUTE_DIR)/, $(EXECUTE_FILES)))

# Object file paths
OBJCS = $(SRCS:.c=.o)

# Build target
all: $(LIBFT) $(NAME)

$(NAME): $(OBJCS)
	$(CC) $(CFLAGS) $(OBJCS) $(LIBFT) $(READLINE) -o $(NAME)

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

# Clean targets
clean:
	rm -f $(OBJCS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all
