# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR)

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
PARSE_FILES = minishell utils greets parser parse_utils parse_utils_2 \
	env_handler syntax_check extra_help signals_handler env_handler_utils env_handler_utils_2 \
	



EXECUTE_FILES = executor exec_buildins_1  exec_buildins_2 exit exit_utils_1  init_env_list \
	export export_utils export_utils_2 unset free cd exce_external exce_external_1

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

.PHONY: all clean fclean re
