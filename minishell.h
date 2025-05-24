/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:41 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/24 21:05:52 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/stat.h>
# include <sys/wait.h>
# include <curses.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <limits.h>

# define INPUT1 "\033[1;96;40mSHELL\033[0m"
# define INPUT2 "\033[1;96;40m ✗ \033[0m"
# define PIPE 1
# define WORD 2
# define REDOUT 3
# define REDIN 4
# define APPEND 5
# define HEDOC 6
# define OPTION 7
# define FILE 8
# define CMD 9
# define ARG 10
# define DEL 11

extern int	g_sig;

struct		s_minishell;
struct		s_commandline;
struct		s_tokenize;

typedef struct s_files
{
	int				delinquotes;
	int				type;
	char			*file;
	char			*delimiter;
	struct s_files	*next;
}	t_files;

typedef struct s_tokenize
{
	int					split;
	int					type;
	int					category;
	int					index;
	char				*str;
	struct s_tokenize	*next;
	struct s_tokenize	*prev;

}	t_tokenize;

typedef struct s_commandline
{
	int						infd;
	int						outfd;
	int						index;
	int						argcount;
	char					*cmd;
	char					**args;
	char					**env;
	t_files					*outfile;
	t_files					*infile;
	struct s_minishell		*mini;
	struct s_commandline	*next;
}	t_commandline;

typedef struct s_env
{
	bool			isexported;
	char			*variable;
	char			*value;
	char			*string;
	struct s_env	*next;
}	t_env;

typedef struct s_minishell
{
	bool			envstate;
	int				linecount;
	int				**pipes;
	int				exitstatus;
	int				expanded;
	int				insingle;
	int				check;
	char			*input;
	t_env			*env;
	t_tokenize		*tokens;
	t_commandline	*commandline;
	t_files			*files;
}	t_minishell;

typedef struct s_builtin
{
	int		i;
	t_env	*new;
	size_t	size;
	char	*var;
	char	*value;
	char	*string;
	char	*tmp;
	char	**str;
}	t_builtin;

typedef struct s_echo
{
	int		i;
	int		optioncheck;
	int		finishedoptions;
	char	*res;
	char	*copy;
	int		size;
}	t_echo;

typedef struct s_expanding
{
	int		total;
	int		insingle;
	int		indouble;
	int		i;
	int		start;
	int		len;
	int		j;
	char	*expandedvar;
	char	*expanded;
	size_t	k;
	char	*var;

}	t_expanding;

typedef struct s_parse
{
	int		total;
	int		insingle;
	int		indouble;
	int		i;
	int		j;
	char	*expanded;
}	t_parse;

typedef struct s_reparse
{
	int			flag;
	int			i;
	int			z;
	int			j;
	int			k;
	char		q;
	char		*token;
	char		*copy;
	char		*string;
	char		*prev;
	int			exportcheck;
}	t_reparse;

typedef struct s_env_setup
{
	char	*var;
	char	*value;
	char	*string;
	int		start;
	int		len;
}	t_env_setup;

typedef struct s_commandline_setup
{
	char			*cmd;
	int				index;
	char			**arg;
	char			*option;
	int				argcount;
	int				count;
	t_commandline	*command;
	t_commandline	*copy;
	t_tokenize		*file;
	t_tokenize		*token;
}	t_commandline_setup;

typedef struct s_handlefiles
{
	t_files	*new;
	t_files	*list;
	int		type;
	int		delquotes;
	char	*file;
	char	*del;
}	t_handlefiles;

typedef struct s_setupnode
{
	int			split;
	int			index;
	char		*str;
}	t_setupnode;

int				ft_strcmp(char *str, char *str1);
int				tokenize(t_minishell *mini);
char			**split(char const *s, char *delims);
int				ft_isalpha(int c);
void			readinput(t_minishell *mini);
t_tokenize		*ft_lstnew(t_tokenize *prev, int cntnt);
char			*ft_strdup(char *str);
void			ft_lstadd_back(t_tokenize **lst, t_tokenize *new);
t_tokenize		*ft_lstlast(t_tokenize *lst);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
void			freedoublearray(char **str);
void			freelisttokens(t_tokenize *list);
char			*ft_strjoin(const char *s1, const char *s2);
size_t			ft_strlen(const char *s);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
void			parse(t_minishell *mini);
char			*handletypes(int i);
void			ft_commandadd_back(t_commandline **lst, t_commandline *new);
t_commandline	*ft_commandlast(t_commandline *lst);
t_commandline	*ft_commandnew(char *cmd, char **arg);
void			freelistcommandline(t_commandline *list);
void			ft_fileadd_back(t_files **lst, t_files *new);
t_files			*ft_filelast(t_files *lst);
t_files			*ft_filenew(char *file, char *del, int type);
void			freelistfiles(t_files *list);
char			*handletypes(int i);
char			*ft_substr(char const *s, unsigned int start, size_t len);
t_env			*ft_envlast(t_env *lst);
t_env			*ft_envnew(char *value, char *var, char *string);
void			ft_envadd_back(t_env **lst, t_env *new);
void			freelistenv(t_env *list);
int				ft_commandsize(t_commandline *lst);
int				ft_envsize(t_env *lst);
void			execute(t_minishell *mini);
void			ft_env(t_minishell *mini, char **args);
void			ft_pwd(t_minishell *mini);
void			ft_putstr_fd(char *s, int fd);
void			ft_cd(t_commandline *commandline);
int				checkcommand(char *cmd);
char			**constructenv(t_env *env);
char			*ft_itoa(int n);
char			*ft_getenv(char *str, t_minishell **mini);
int				isanoption(char *str);
void			ft_echo(t_commandline *command);
void			ft_bzero(void *s, size_t n);
void			initializetonone(char **str, int len);
char			*ft_strtrim(char *s1, char *set);
int				getinput(int delflag, char *del, t_minishell *mini);

void			unset(t_commandline *commandline);
void			syntaxhere(int *check, char *flag, int print);
void			syntax(int *check, char *flag, int print);
int				countheredocs(t_tokenize *tokens);
void			reparse(t_minishell *mini);
char			*expand(int *check, char *str, t_minishell *mini);
int				ft_isdigit(int c);
void			ft_exit(t_commandline *command);
void			export(t_commandline *command);
char			*fillspace(const char *input);
int				is_delim(char c, char *delims);
void			signalhandle(int sig);
void			callallsignals(void);
void			quithandle(int sig);
void			heredochandle(int sig);
t_env			*getenvnode(t_env *env, char *var);
int				is_directory(char *path);
void			directoryerror(char *s);
char			*ft_strchr(const char *s, int c);
int				checkvalid(char *str, t_minishell *mini);
int				ft_cdhelp(char *oldcwd, int size);
int				ft_cdhelp2(int size, t_commandline *command, char *oldcwd);
int				hel(char *targetdir, t_minishell *mini, int size, char *oldcwd);
int				ft_cdhelp4(char *targetdir, int size, char *oldcwd);
int				ft_cdhelp5(char *pwd, int size, char *oldcwd);
int				checkvalid(char *str, t_minishell *mini);
int				ft_isalnummm(int c);
int				ft_isalphaa(int c);
char			*getvalue(char *str);
char			*getvar(char *str);
char			*getstring(char *str);
t_env			*getenvnode(t_env *env, char *var);
void			exporthelp(t_commandline *command, t_builtin *export);
void			exporthelp2(t_commandline *command, t_builtin *export);
void			exporthelp3(t_commandline *command, t_builtin *export);
int				atoihelp(int *overflow, long long result, long long old_result);
long long		ft_atoi(char *str, int *overflow);
char			*ft_join(const char *s1, const char *s2);
void			echo_loop(t_commandline *command, t_echo *echo);
void			fillfirst(const char *s1, char *join, int i);
char			*handle(char *str, char *str1);
void			syntaxhere(int *check, char *flag, int print);
void			syntax(int *check, char *flag, int print);
int				parsepipe(t_tokenize *list);
int				parseinput(t_tokenize *list);
void			setupnode(t_setupnode *setup,
					int category, int type, t_tokenize **tokens);
int				parseoutput(t_tokenize *list);
void			retokenize(t_minishell *mini);
int				tokenizewordshelp(t_tokenize **list);
void			tokenizewords(t_minishell *mini);
int				isanoption(char *str);
char			*handletypes(int i);
bool			is_token(char c);
char			*fillspace(const char *input);
int				ft_isalnum(int c);
char			*ft_strtrim(char *s1, char *set);
int				countdouble(char *str, char *delims);
int				countword(char **str);
void			handlenodes(int split, char *str, int i, t_tokenize **tokens);
char			*alloc(char *str);
int				in_set(char c, char *set);
char			*removequotes(int *flag, char *str);
int				in_set(char c, char *set);
void			initreparse(t_reparse *reparse, char *str);
char			*ft_getenv(char *str, t_minishell **mini);
int				ft_isalnumm(int c);
int				handle_variable_expansion(char *str, int *i, t_minishell *mini);
int				handle_dollar_sign(char *str, int *i, t_minishell *mini);
int				varlen(char *str, t_minishell *mini);
int				expandhelp(t_expanding *expand, char *str,
					int *check, t_minishell *mini);
void			initilizeexpand(t_minishell *mini, char *str,
					t_expanding *expand);
int				quoted(char *str);
void			quotedhelper(char quote, char *str, int *i, int *len);
int				ft_isdigit(int c);
size_t			str_length(char const *s, char *delims);
size_t			count_str(char *s, char *delims);
int				ft_isspace(char c);
void			countargshelp(char *args, int *i, int *count, int *in_word);
int				countargs(char *args, char *option);
int				addfilehelp(t_tokenize **copy, t_commandline **command);
void			addfile(t_tokenize *token, t_commandline *commandline);
void			handlefiles(t_tokenize *token, t_commandline *command);
int				getarguments(t_tokenize *tokens);
void			freedoubleint(t_minishell *mini);
void			heredochandle(int sig);
int				openallfiles(t_minishell *mini);
void			setupcommandline(t_minishell *mini);
void			checkheredocs(t_minishell *mini);
void			handlefiles(t_tokenize *token, t_commandline *command);
void			getinputhelp(char *line, int fd);
void			getinputhelp1(int delflag, char **line,
					char *copy, t_minishell *mini);
void			getinputhelp2(int *fd, t_minishell **mini, char *line);
int				getinputhelp3(char *line, t_minishell *mini, char *del);
int				getinputhelp4(char *line, char *del);
void			initializepipes(t_minishell *mini);
void			childlabor(t_commandline **command);
void			handleiosingle(t_commandline **command);
void			closeallpipes(t_minishell *mini, int size);
void			handlebuiltins(t_commandline **command);
void			setupchilds(t_minishell *mini, int size);
int				openfiles(t_commandline **command, t_minishell *mini);
void			freestr(char **str);
char			*my_getenv(t_minishell *mini, char *str);
void			handleiolast(t_commandline **command);
void			setup_io(t_commandline *command, int size);
void			error_check(t_commandline *command);
void			after_execve(t_commandline *command);
void			closeallfiles(t_minishell *mini);
void			error(char *str);
int				ft_find(char *str, char *del);
char			*checkfile(t_commandline *command);
void			printerror(char *str);
int				ft_atoi_custom(const char *str);
void			handle_shlvl(t_commandline *command);
void			setuplastcommand(t_commandline ***command);
void			setupmiddlecommand(t_commandline ***command);
void			tokenhelper(t_tokenize **list);
void			file_helper(int flag, char *str,
					t_commandline **command, int fd);
void			startpipex(t_minishell *mini);
void			setupenv(char **env, t_minishell *mini);

#endif