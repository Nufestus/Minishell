/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:41 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/01 20:11:42 by rammisse         ###   ########.fr       */
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

# define INPUT1 "SHELL ✗ "
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
# define OUT 12
# define IN 13

extern int	g_sig;

struct		s_minishell;
struct		s_commandline;
struct		s_tokenize;

typedef struct s_setupchild
{
	int						i;
	int						sig;
	int						skip;
	int						status;
	struct s_commandline	*copy;
}			t_setupchild;

typedef struct s_tokenexpect
{
	int		expect_cmd;
	int		expect_file;
	int		expect_delim;
}			t_tokenexpect;

typedef struct s_files
{
	int				fd;
	int				hedoc;
	int				redir;
	int				delinquotes;
	int				type;
	char			*file;
	char			*delimiter;
	struct s_files	*next;
}	t_files;

typedef struct s_tokenize
{
	int					flag;
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
	pid_t					pid;
	int						iscmdexpand;
	int						infd;
	int						outfd;
	int						index;
	int						argcount;
	char					*cmd;
	char					**args;
	char					**env;
	t_files					*file;
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
	int				flag;
	int				**pipes;
	int				exitstatus;
	int				expanded;
	int				insingle;
	int				check;
	char			*pwd;
	char			*input;
	t_env			*env;
	t_tokenize		*tokens;
	t_commandline	*commandline;
	t_files			*files;
}	t_minishell;

typedef struct s_builtin
{
	int		i;
	int		isexported;
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
	char		**s;
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
	int				cmdcheck;
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

int				getinputhelp3(char *line, t_minishell *mini, char *del);
int				getinputhelp4(char *line, char *del);
int				getinput(int delflag, char *del, t_minishell *mini);
int				readinputhelp(t_minishell **mini);
int				readinputhelp1(t_minishell **mini);
int				ft_reparsehelp(t_reparse *reparse, char *str, int *check);
int				tokenize(t_minishell *mini);
int				is_delim(char c, char *delims);
int				ft_reparsehelp1(t_reparse *reparse, t_minishell *mini);
int				ft_commandsize(t_commandline *lst);
int				ft_envsize(t_env *lst);
int				ft_strcmp(char *str, char *str1);
int				ft_isalpha(int c);
int				ft_isspace(char c);
int				countheredocs(t_tokenize *tokens);
int				countargs(char *args, char *option);
int				addfilehelp(t_tokenize **copy, t_commandline **command);
int				getarguments(t_tokenize *tokens);
int				openallfiles(t_minishell *mini);
int				atoihelp(int *overflow, long long result, long long old_result);
int				ft_cdhelp(char *oldcwd, int size,
					t_commandline *command, t_minishell *mini);
int				ft_cdhelp2(int size, t_commandline *command, char *oldcwd);
int				hel(char **targetdir, t_minishell *mini,
					int size, char *oldcwd);
int				isnotnum(char *str);
int				exithelp(int flag, t_commandline *command);
int				exithelp2(t_commandline *command);
int				ft_isalnummm(int c);
int				ft_isalphaa(int c);
int				checkvalid(char *str, t_minishell *mini);
int				is_directory(char *path);
int				checkcommand(char *cmd);
int				ft_atoi_custom(const char *str);
int				openfiles(t_commandline **command, t_minishell *mini);
int				ft_find(char *str, char *del);
int				setexit(int exitstatus, int flag);
int				ft_isalnumm(int c);
int				handle_variable_expansion(char *str, int *i, t_minishell *mini);
int				ft_cd(t_commandline *command);
int				handle_dollar_sign(char *str, int *i, t_minishell *mini);
int				varlen(char *str, t_minishell *mini);
int				ft_isdigit(int c);
int				retokhelp(int *seen_cmd, int *next_is_file,
					int *next_is_delim, t_tokenize **tok);
int				quoted(char *str);
int				anoption(char *str);
int				expandhelp(t_expanding *expand, char *str,
					int *check, t_minishell *mini);
int				expandhelp2(t_expanding *expand, char *str, int *check);
int				expandhelp3(t_expanding *expand, char *str,
					t_minishell *mini, int *check);
int				parsepipe(t_tokenize *list);
int				parseinput(t_tokenize *list);
int				parseoutput(t_tokenize *list);
int				isanoption(char *str);
int				ft_isalnum(int c);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				countdouble(char *str, char *delims);
int				countword(char **str);
int				addfile(t_tokenize *token, t_commandline *commandline);
int				count_len(int n);
int				in_set(char c, char *set);
int				removequoteshelp(int *flag, t_parse *parse, char *str);
int				handlefiles(t_tokenize *token, t_commandline *command);
int				setupcommandline(t_minishell *mini);
int				exportsorted(t_commandline *command);
int				printarrayhelp(char **array, int *i, int *j);
char			*ft_getenv(char *str, t_minishell *mini);
char			*expand(int *check, char *str, t_minishell *mini);
char			*fillspace(const char *input);
char			*ft_strtrim(char *s1, char *set);
char			*rtrn(int n, char *num, size_t len);
char			*ft_join(const char *s1, const char *s2);
char			*handle(char *str, char *str1);
char			*getvalue(char *str);
char			*getvar(char *str);
char			*getstring(char *str);
char			*checkfile(t_commandline *command);
char			**constructenv(t_env *env);
char			*handle(char *str, char *str1);
char			*my_getenv(t_minishell *mini, char *str);
char			**tosortarray(t_commandline *command);
char			**sortarray(char **str);
char			*expandinheredoc(int *check, char *str, t_minishell *mini);
char			**split(char const *s, char *delims);
char			*ft_strjoin(const char *s1, const char *s2);
char			*ft_strdup(char *s1);
char			*ft_strchr(const char *s, int c);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			**ft_strdupdouble(char *cmd, char **str);
char			**create_default_env(void);
char			*ft_itoa(int n);
char			*alloc(char *str);
char			*removequotes(int *flag, char *str);
char			*ft_getenvv(char *str, t_minishell **mini);
char			*no_file(char *str, t_commandline *command);
bool			is_token(char c);
void			directory_free(t_commandline *command);
void			echo_loop(t_commandline *command, t_echo *echo);
void			ft_setenv(char *envname, char *newvalue, t_minishell *mini);
void			ft_cdhelp6(int size, char *pwd, char *oldcwd);
void			fillfirst(const char *s1, char *join, int i);
void			helpecho(int size, int optioncheck,
					char *res, t_commandline *command);
void			initechovars(int *i, int *optioncheck,
					int *finishedoptions, char **res);
void			ft_echo(t_commandline *command);
void			ft_putstr_fd(char *s, int fd);
void			ft_pwdhelp(int size, char *path, t_minishell *mini);
void			ft_pwd(t_minishell *mini, t_commandline *command);
void			ft_envhelp(int size, t_env *env, t_minishell *mini);
void			ft_env(t_minishell *mini, char **args);
void			ft_exit(t_commandline *command);
void			exporthelp(t_commandline *command, t_builtin *export);
void			exporthelp2(t_commandline *command, t_builtin *export);
void			exporthelp3(t_commandline *command, t_builtin *export);
void			exporthelp4(t_commandline *command, t_builtin *export);
void			exportstatements(t_commandline *command, t_builtin *export);
void			export(t_commandline *command);
void			freeunset(t_env *curr);
void			checkset(char *res, int *check);
void			lstremoveif(t_env **env, char *value);
void			unset(t_commandline *commandline);
void			directoryerror(char *s);
void			initializepipes(t_minishell *mini);
void			printerror(char *str);
void			error(char *str);
void			checkthenext(int *seen_cmd, int *next_is_delim,
					int *next_is_file, t_tokenize **tok);
void			handlepoint(t_commandline *command);
void			handlebuiltins(t_commandline **command);
void			closeallfiles(t_minishell *mini);
void			handleiolast(t_commandline **command);
void			setuplastcommand(t_commandline ***command);
void			handleiomiddle(t_commandline **command);
void			envhelp(int size, t_minishell *mini);
void			setupmiddlecommand(t_commandline ***command);
void			handleiosingle(t_commandline **command);
void			setup_io(t_commandline **command, int size);
void			error_check(t_commandline *command);
void			handle_shlvl(t_commandline *command);
void			after_execve(t_commandline *command);
void			setupfirstcommand(t_commandline ***command);
void			childlabor(t_commandline **command);
void			closeallpipes(t_minishell *mini, int size);
void			setupchilds(t_minishell *mini, int size);
void			freestr(char **str);
void			add_to_env(char *str, char *s, t_minishell *mini);
void			startpipex(t_minishell *mini);
void			execute(t_minishell *mini);
void			handle_dir_error(char *arg, char *oldpwd, t_minishell *mini);
void			safe_exit(int status);
void			handlesig(t_setupchild *child);
void			signalhandle(int sig);
void			callallsignals(void);
void			sigdfl(void);
void			normalhande(int sig);
void			quotedhelper(char quote, char *str, int *i, int *len);
void			initilizeexpand(t_minishell *mini,
					char *str, t_expanding *expand);
void			expandhelp1(t_expanding *expand, char *str);
void			setupnode(t_setupnode *setup,
					int category, int type, t_tokenize **tokens);
void			syntaxhere(int *check, char *flag, int print);
void			syntax(int *check, char *flag, int print);
void			retokenize(t_minishell *mini);
void			fillspacehelp(int *j, int *i, const char *input, char *out);
void			fillspacehelp2(int *j, int *i, char *out, const char *input);
void			countwordhelp(int *i, char *s, char q, int *k);
void			handlenodes(int split, char *str, int i, t_minishell *mini);
void			initreparse(t_reparse *reparse, char *str);
void			reparsehelp(t_tokenize *list, int *check,
					t_minishell *mini, char *del);
void			reparse(t_minishell *mini);
void			parsehelp(t_tokenize *list, int *check);
void			parse(t_minishell *mini);
void			countargshelp(char *args, int *i, int *count, int *in_word);
void			freedoubleint(t_minishell *mini);
void			heredochandle(int sig);
void			commandlinehelp(t_commandline_setup *setup);
void			commandlinehelp1(t_commandline_setup *setup, t_minishell *mini);
void			initvarscmd(t_commandline_setup *setup, t_minishell *mini);
void			settozero(t_commandline_setup *setup);
void			heredocsnorm(t_tokenize **list, int *count, int *check);
void			checkheredocs(t_minishell *mini);
void			handlefileshelp(t_handlefiles *files, t_tokenize **token);
void			getinputhelp(char *line, int fd);
void			initializechild(t_setupchild *child,
					int size, t_minishell *mini);
void			getinputhelp1(int delflag, char **line,
					char *copy, t_minishell *mini);
void			getinputhelp2(int *fd, t_minishell **mini, char *line);
void			handlesuchfile(char *str, t_commandline *command);
void			readinput(t_minishell *mini);
void			free_exit(t_minishell *mini, int stat);
void			ft_reparsehelp2(t_reparse *reparse,
					char **s, t_minishell *mini);
void			ft_reparse(int *check, char *str, t_minishell *mini);
void			ft_lstadd_back(t_tokenize **lst, t_tokenize *new);
void			closeheredocs(t_files *files);
void			closeallheredocs(t_minishell *mini);
void			freedoublearray(char **str);
void			freelistcommandline(t_commandline *list);
void			freelistfiles(t_files *list);
void			freelistenv(t_env *list);
void			ft_bzero(void *s, size_t n);
void			ft_fileadd_back(t_files **lst, t_files *new);
void			handlesuchfile(char *str, t_commandline *command);
void			ft_envadd_back(t_env **lst, t_env *new);
void			ft_commandadd_back(t_commandline **lst, t_commandline *new);
void			initializetonone(char **str, int len);
void			setupenv(char **env, t_minishell *mini);
void			handleforkmiddle(t_commandline **command, pid_t pid, int size);
void			handleforklast(t_commandline **command, pid_t pid, int size);
void			freelisttokens(t_tokenize *list);
void			closeheredocs(t_files *files);
void			printarray(char **array);
void			exporthelp5(t_commandline *command, t_builtin *export);
t_env			*getenvnode(t_env *env, char *var);
t_env			*ft_envlast(t_env *lst);
t_env			*ft_envnew(char *value, char *var, char *string);
size_t			count_str(char *s, char *delims);
size_t			str_length(char const *s, char *delims);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t			ft_strlen(const char *s);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
t_files			*ft_filelast(t_files *lst);
t_files			*ft_filenew(char *file, char *del, int type);
long long		ft_atoi(char *str, int *overflow);
t_tokenize		*ft_lstlast(t_tokenize *lst);
t_tokenize		*ft_lstnew(t_tokenize *prev, int cntnt);
t_commandline	*ft_commandlast(t_commandline *lst);
t_commandline	*ft_commandnew(char *cmd, char **arg);

#endif