# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/21 12:25:14 by aammisse          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2025/02/25 16:22:36 by rammisse         ###   ########.fr        #
=======
#    Updated: 2025/02/25 14:41:27 by aammisse         ###   ########.fr        #
>>>>>>> refs/remotes/origin/main
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

<<<<<<< HEAD
SRCS = minishell.c parsing/readinput.c parsing/ft_split.c utils/utils.c parsing/tokenize.c
=======
SRCS = minishell.c parsing/readinput.c parsing/ft_split.c utils/split.c
>>>>>>> refs/remotes/origin/main

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