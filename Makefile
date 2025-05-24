# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/21 12:25:14 by aammisse          #+#    #+#              #
#    Updated: 2025/05/24 00:43:05 by aammisse         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g

SRCS = minishell.c parsing/readinput.c utils/utils.c builtins/export.c \
		parsing/tokenize.c utils/strjoin.c utils/split.c parsing/parsing.c \
		execution/execution.c builtins/env_pwd.c builtins/cd.c \
		parsing/expanding.c builtins/echo.c builtins/unset.c builtins/exit.c \
		parsing/ft_split.c execution/signals.c execution/exec_utils.c \
		execution/exec_utils_1.c execution/exec_utils_2.c execution/exec_utils_3.c \
		execution/exec_utils_4.c execution/exec_utils_5.c

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