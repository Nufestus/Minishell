/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:41 by aammisse          #+#    #+#             */
/*   Updated: 2025/02/26 22:07:36 by rammisse         ###   ########.fr       */
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

#define PIPE 1
#define APPEND 2
#define REDOUT 3
#define REDIN 4
#define WORD 5
#define HEDOC 6

typedef struct s_tokenize
{
	int					type;
	char				*string;
	struct s_tokenize	*next;
	
} t_tokenize;
 
typedef struct s_minishell
{
	char **env;
	char *input;
	char **commandline;c
}               t_minishell;

typedef struct s_commandline
{
	int type;
	char *content;
	struct s_commandline *next;
}           t_commandline;

char		**ft_split(char *s, char *delims);
char		**split(char const *s, char *delims);
void		readinput(t_minishell *mini);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
t_tokenize	*ft_lstnew(int type);
t_tokenize	*ft_lstlast(t_tokenize *lst);
void		ft_lstadd_back(t_tokenize **lst, t_tokenize *new);
void		tokenize(t_minishell *mini);

#endif