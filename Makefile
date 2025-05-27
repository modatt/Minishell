# # Compiler and flags
# CC = cc
# CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR)

# # Directories
# SRC_DIR = srcs
# INC_DIR = includes
# LIBFT_DIR = libft

# # Libraries
# LIBFT = $(LIBFT_DIR)/libft.a
# READLINE = -lreadline

# # Source and object files
# SRCS = ./$(SRC_DIR)/minishell.c ./$(SRC_DIR)/utils.c
# OBJCS = $(SRCS:.c=.o)

# # Output program name
# NAME = minishell

# # Build all
# all: $(LIBFT) $(NAME)

# # Build minishell
# $(NAME): $(OBJCS)
# 	$(CC) $(CFLAGS) $(OBJCS) $(LIBFT) $(READLINE) -o $(NAME)

# # Build libft
# $(LIBFT):
# 	make -C $(LIBFT_DIR)

# clean:
# 	rm -f $(OBJCS)
# 	make -C $(LIBFT_DIR) clean

# fclean: clean
# 	rm -f $(NAME)
# 	make -C $(LIBFT_DIR) fclean

# re: fclean all

# .PHONY: all clean fclean re


CC = cc 
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) #add/ connect header

# DIR

SRCS_DIR = srcs
INC_DIR = includes
LIBFT_DIR = libft

#llibraries
READLINE = -lreadline
LIBFT = $(LIBFT_DIR)/libft.a

# SRCS && OBJ
SRCS = ./$(SRCS_DIR)/minishell.c ./$(SRCS_DIR)/utils.c ./$(SRCS_DIR)/parser.c
OBJCS = $(SRCS:.c=.o)

NAME = minishell


#BUILD 
all: $(LIBFT) $(NAME) 

$(NAME): $(OBJCS)
	@$(CC) $(CFLAGS) $(OBJCS) $(LIBFT) $(READLINE) -o $(NAME)
$(LIBFT):
	@make -s -C $(LIBFT_DIR)

clean:
	rm -f $(OBJCS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(OBJCS)
	make -C $(LIBFT_DIR) fclean
re: fclean all
.PHONY: all clean fclean re
