/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:41 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/12 21:05:01 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
#define MINISHELL_H

#include <pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <curses.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include "utils/GNL/get_next_line.h"

#define INPUT1 "\033[1;96;40mSHELL\033[0m"
#define INPUT2 "\033[1;96;40m ✗ \033[0m"
#define PIPE 1
#define WORD 2
#define REDOUT 3
#define REDIN 4
#define APPEND 5
#define HEDOC 6
#define OPTION 7
#define FILE 8
#define CMD 9
#define ARG 10
#define DEL 11

struct s_minishell;
struct s_commandline;
struct s_tokenize;

typedef struct s_files
{
	int		type;
	char	*file;
	char	*delimiter;
	struct s_files *next;
}	t_files;

typedef struct s_tokenize
{
	int					type;
	int					category;
	int					index;
	char				*str;
	struct s_tokenize	*next;
	struct s_tokenize	*prev;
	
} t_tokenize;

typedef struct s_commandline
{
	int		infd;
	int		outfd;
	int		index;
	int argcount;
	char *cmd;
	char **args;
	char **env;
	t_files *outfile;
	t_files *infile;
	struct s_minishell *mini;
	struct s_commandline *next;
}           t_commandline;

typedef struct s_env
{
	bool	isexported;
	char *variable;
	char *value;
	char *string;
	struct s_env *next;
}				t_env;

typedef struct s_minishell
{
	int	**pipes;
	int exitstatus;
	int check;
	char *input;
	t_env *env;
	t_tokenize *tokens;
	t_commandline *commandline;
}               t_minishell;

char    **ft_split(int *check, char *s, char *delims);
int ft_strcmp(char *str, char *str1);
int	tokenize(t_minishell *mini);
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
char	*ft_strjoin(const char *s1, const char *s2);
size_t	ft_strlen(const char *s);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void	parse(t_minishell *mini);
char *handletypes(int i);
void	ft_commandadd_back(t_commandline **lst, t_commandline *new);
t_commandline	*ft_commandlast(t_commandline *lst);
t_commandline	*ft_commandnew(char *cmd, char *option, char **arg);
void freelistcommandline(t_commandline *list);
void	ft_fileadd_back(t_files **lst, t_files *new);
t_files	*ft_filelast(t_files *lst);
t_files	*ft_filenew(char *file, char *del, int type);
void freelistfiles(t_files *list);
char *handletypes(int i);
char	*ft_substr(char const *s, unsigned int start, size_t len);
t_env	*ft_envlast(t_env *lst);
t_env	*ft_envnew(char *value, char *var, char *string);
void	ft_envadd_back(t_env **lst, t_env *new);
void freelistenv(t_env *list);
int	ft_commandsize(t_commandline *lst);
int	ft_envsize(t_env *lst);
char **expanding(char **strs, t_minishell *mini);
void execute(t_minishell *mini);
void ft_env(t_minishell *mini, char **args);
void ft_pwd(t_minishell *mini);
void	ft_putstr_fd(char *s, int fd);
void ft_cd(t_commandline *commandline);
int checkcommand(char *cmd);
char **constructenv(t_env *env);
void handleiosingle(t_commandline *command);
int	openfiles(t_commandline *command);
char **expanding(char **strs, t_minishell *mini);
char	*ft_itoa(int n);
char *ft_getenv(char *str, t_minishell *mini);
int isanoption(char *str);
void ft_echo(t_commandline *command);
void	ft_bzero(void *s, size_t n);
void initializetonone(char **str, int len);
char	*ft_strtrim(char *s1, char *set);
int getinput(char *del);
void heredocerror(char *str);
void unset(t_commandline *commandline);
void syntaxhere(int *check, char *flag, int print);
void syntax(int *check, char *flag, int print);
int countheredocs(t_tokenize *tokens);
void	reparse(t_minishell *mini);

#endif