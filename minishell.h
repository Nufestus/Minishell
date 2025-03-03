/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:41 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/03 00:46:00 by aammisse         ###   ########.fr       */
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
#define WORD 2
#define REDOUT 3
#define REDIN 4
#define APPEND 5
#define HEDOC 6
#define OPTION 7

struct s_minishell;
struct s_commandline;
struct s_tokenize;

typedef struct s_tokenize
{
	int					type;
	int					index;
	char				*str;
	struct s_tokenize	*next;
	struct s_tokenize	*prev;
	
} t_tokenize;

typedef struct s_minishell
{
	char **env;
	char *input;
	t_tokenize *tokens;
}               t_minishell;

typedef struct s_commandline
{
	int type;
	char *content;
	struct s_commandline *next;
}           t_commandline;

char    **ft_split(char const *s, char *delims);
int ft_strcmp(char *str, char *str1);
void tokenize(t_minishell *mini);
char    **split(char const *s, char *delims);
int	ft_isalpha(int c);
void readinput(t_minishell *mini);
t_tokenize	*ft_lstnew(t_tokenize *prev, int cntnt);
char *ft_strdup(char *str);
void	ft_lstadd_back(t_tokenize **lst, t_tokenize *new);
t_tokenize	*ft_lstlast(t_tokenize *lst);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
void	freedoublearray(char **str);
void freelisttokens(t_tokenize *list);

#endif