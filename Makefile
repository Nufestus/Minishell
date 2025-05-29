# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/21 12:25:14 by aammisse          #+#    #+#              #
#    Updated: 2025/05/29 18:15:11 by rammisse         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = minishell.c parsing/readinput.c utils/utils.c builtins/export.c \
		parsing/tokenize.c utils/strjoin.c utils/split.c parsing/parsing.c \
		execution/execution.c builtins/env_pwd.c builtins/cd.c \
		parsing/expanding.c builtins/echo.c builtins/unset.c builtins/exit.c \
		execution/signals.c utils/utils1.c utils/utils2.c parsing/readinput_utils3.c \
		utils/utils3.c utils/utils4.c utils/utils5.c builtins/export_utils_1.c \
		builtins/export_utils_2.c builtins/cd_utils.c builtins/builtin_utils.c \
		parsing/parsing_utils.c parsing/parsing_utils1.c parsing/parsing_utils2.c \
		parsing/parsing_utils3.c parsing/parsing_utils4.c parsing/parsing_utils5.c \
		parsing/expanding_utils.c parsing/expanding_utils1.c parsing/readinput_utils.c \
		parsing/readinput_utils1.c parsing/readinput_utils2.c parsing/readinput_utils4.c \
		parsing/readinput_utils5.c execution/exec_utils.c execution/exec_utils_1.c \
		execution/exec_utils_2.c execution/exec_utils_3.c execution/exec_utils_4.c execution/exec_utils_5.c \
		main.c builtins/export_stuff.c execution/exec_utils_6.c
		

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME)

$(OBJS): %.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all