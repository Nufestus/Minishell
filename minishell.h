/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:41 by aammisse          #+#    #+#             */
/*   Updated: 2025/02/25 00:22:10 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <curses.h>
#include <termios.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INPUT1 "\033[1;96;40mSHELL\033[0m"
#define INPUT2 "\033[1;96;40m ✔ \033[0m"

typedef struct s_tokenize
{
	int					type;
	struct s_tokenize	*next;
	
} t_tokenize;

typedef struct s_minishell
{
	char *input;
}               t_minishell;

typedef struct s_commandline
{
	int type;
	char *content;
	struct s_commandline *left;
	struct s_commandline *right;
	struct s_commandline *root;
}           t_commandline;

char    **ft_split(char const *s, char *delims);
char    **split(char const *s, char *delims);
void readinput(t_minishell *mini);

#endif