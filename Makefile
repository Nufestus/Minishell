# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/21 12:25:14 by aammisse          #+#    #+#              #
#    Updated: 2025/05/13 00:21:34 by rammisse         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = minishell.c parsing/readinput.c parsing/ft_split.c utils/utils.c \
		parsing/tokenize.c utils/strjoin.c utils/split.c parsing/parsing.c \
		execution/execution.c builtins/env_pwd.c builtins/cd.c \
		parsing/expanding.c builtins/echo.c utils/GNL/get_next_line.c \
		utils/GNL/get_next_line_utils.c builtins/unset.c

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