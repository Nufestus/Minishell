# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/21 12:25:14 by aammisse          #+#    #+#              #
#    Updated: 2025/04/27 04:37:10 by aammisse         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3

SRCS = minishell.c parsing/readinput.c parsing/ft_split.c utils/utils.c parsing/tokenize.c utils/strjoin.c utils/split.c parsing/parsing.c execution/execution.c

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