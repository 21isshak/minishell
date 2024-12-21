# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/29 20:11:36 by iskaraag          #+#    #+#              #
#    Updated: 2024/12/17 16:54:32 by iskaraag         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

OBJ_DIR = objects
SRC_DIR = src
SRCS = ft_string_utils.c path_utils.c parse_utils.c builtin_handlers.c builtin_execution.c signal_handling.c input_handling.c execution.c main.c
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
CC = clang
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline
RM = rm -rf
NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
		@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)
		@echo "\033[1;35mREADY TO GO!\033[0m"
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
		@$(CC) $(CFLAGS) -c $< -o $@
		
$(OBJ_DIR):
		@mkdir -p $(OBJ_DIR)

clean:
		@$(RM) $(OBJS)
		@$(RM) $(OBJ_DIR)
		@echo "\033[1;33mcleane: DONE!\033[0m"

fclean: clean
		@$(RM) $(NAME)
		@echo "\033[1;33mfcleane: DONE!\033[0m"

re: fclean all

.PHONY: all clean fclean re
