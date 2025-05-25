/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:06:43 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/24 23:05:15 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int g_sig = 0;

int	ft_isspace(char c)
{
	return (c == ' '  || c == '\t' 
		|| c == '\n' || c == '\v' 
		|| c == '\f' || c == '\r');
}

int	countargs(char *args, char *option)
{
	int i;
	int count;
	int in_word;

	if (!args || !option)
		return (0);
	i = 0;
	count = 0;
	in_word = 0;
	while (args[i])
	{
		if (!ft_isspace(args[i]) && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (ft_isspace(args[i]))
			in_word = 0;
		i++;
	}
	i = 0;
	while (option[i])
	{
		if (!ft_isspace(option[i]) && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (ft_isspace(option[i]))
			in_word = 0;
		i++;
	}
	return (count + 1);
}

void handlefiles(t_tokenize *token, t_commandline *command)
{
	t_files *new;
	t_files *list;
	int type;
	int	delquotes;
	char *file;
	char *del;

	type = 0;
	file = NULL;
	del = NULL;
	new = NULL;
	list = NULL;
	delquotes = 0;
	if ((token->next->split == 2 || token->next->split == 3) && token->type == HEDOC)
		token->split = token->next->split;
	if (token->type == REDIN || token->type == APPEND
			|| token->type == REDOUT)
	{
		type = token->type;
		file = ft_strdup(token->next->str);
	}
	else if (token->type == HEDOC)
	{
		if (token->split == 2 || token->split == 3)
			delquotes = 1;
		type = token->type;
		del = ft_strdup(token->next->str);
	}
	new = ft_filenew(file, del, type);
	new->delinquotes = delquotes;
	if (token->type == REDIN || token->type == HEDOC)
		ft_fileadd_back(&command->infile, new);
	if (token->type == REDOUT || token->type == APPEND)
		ft_fileadd_back(&command->outfile, new);
	free(file);
	free(del);
}

void addfile(t_tokenize *token, t_commandline *commandline)
{
	t_tokenize *copy;
	t_commandline *command;

	copy = token;
	command = commandline;
	while(copy)
	{
		if (copy->index == 0)
		{
			while (copy && command && copy->type != PIPE)
			{
				if (copy->category)
					handlefiles(copy, command);
				copy = copy->next;
			}
			command = command->next;
		}
		else if (copy->type == PIPE)
		{
			copy = copy->next;
			if (!copy)
				break ;
			while (copy && command && copy->type != PIPE)
			{
				if (copy->category)
					handlefiles(copy, command);
				copy = copy->next;
			}
			command = command->next;
		}
	}
}

int getarguments(t_tokenize *tokens)
{
	int res;
	t_tokenize *token;

	res = 0;
	token = tokens;
	while(token && token->type != PIPE)
	{
		res++;
		token = token->next;
	}
	return (res);
}

void	setupcommandline(t_minishell *mini)
{
	char *cmd;
	int index;
	char **arg;
	char *option;
	int argcount;
	int count;
	t_commandline *command;
	t_commandline *copy;
	t_tokenize *file;
	t_tokenize *token;

	token = mini->tokens;
	file = mini->tokens;
	command = NULL;
	copy = NULL;
	mini->commandline = NULL;
	index = 0;
	while(token)
	{
		count = 0;
		cmd = NULL;
		arg = NULL;
		option = NULL;
		argcount = 0;
		if (token->index == 0)
		{
			count = getarguments(token);
			arg = malloc(sizeof(char *) * (count + 3));
			while (token && token->type != PIPE)
			{
				if (token->type == CMD)
					cmd = token->str;
				else if (token->type == ARG)
					arg[argcount++] = ft_strdup(token->str);
				else if (token->type == OPTION)
					arg[argcount++] = ft_strdup(token->str);
				token = token->next;
			}
			arg[argcount] = NULL;
			command = ft_commandnew(cmd, arg);
			command->index = index;
			command->mini = mini;
			command->argcount = argcount;
			ft_commandadd_back(&mini->commandline, command);  
			freedoublearray(arg);
			index++;
		}
		else if (token->type == PIPE)
		{
			token = token->next;
			if (!token)
				break ;
			count = getarguments(token);
			arg = malloc(sizeof(char *) * (count + 3));
			while (token && token->type != PIPE)
			{
				if (token->type == CMD)
					cmd = token->str;
				else if (token->type == ARG)
					arg[argcount++] = ft_strdup(token->str);
				else if (token->type == OPTION)
					arg[argcount++] = ft_strdup(token->str);
				token = token->next;
			}
			arg[argcount] = NULL;
			command = ft_commandnew(cmd, arg);
			command->index = index;
			command->mini = mini;
			command->argcount = argcount;
			ft_commandadd_back(&mini->commandline, command);
			freedoublearray(arg);
			index++;
		}
	}
	addfile(file, mini->commandline);
	// int i;
	// t_files *wtf;
	// t_files *idk;
	// printf("\n\n\n");
	// if (mini->commandline)
	// {
	// 	copy = mini->commandline;
	// 	while(copy)
	// 	{
	// 		printf("Commandline %d:\n", copy->index);
	// 		idk = copy->infile;
	// 		wtf = copy->outfile;
	// 		printf("----------------------------------------------------\n");
	// 		if (copy->cmd)
	// 			printf("Cmd: %s\nNumber of Args:%d\nArgs: ", copy->cmd, copy->argcount);
	// 		else
	// 			printf("Cmd: %s\nNumber of Args:%d\nArgs: ", "no cmd", copy->argcount);
	// 		i = 0;
	// 		if (!copy->args)
	// 			printf("(null)\n");
	// 		else
	// 		{
	// 			while(copy->args[i])
	// 				printf("%s , ", copy->args[i++]);
	// 		}
	// 		printf("\nInfiles:\n");
	// 		if (!idk)
	// 			printf("NONE");
	// 		while(idk)
	// 		{
	// 			if (idk->file)
	// 				printf(" | file : %s --> type : %s | ", idk->file, handletypes(idk->type));
	// 			if (idk->delimiter)
	// 				printf(" | Del : %s  | ", idk->delimiter);
	// 			idk = idk->next;
	// 		}
	// 		printf("\nOutfiles:\n");
	// 		if (!wtf)
	// 			printf("NONE");
	// 		while(wtf)
	// 		{
	// 			if (wtf->file)
	// 				printf(" | file : %s --> type : %s |", wtf->file, handletypes(wtf->type));
	// 			wtf = wtf->next;
	// 		}
	// 		printf("\n----------------------------------------------------\n\n");
	// 		printf("\n\n");
	// 		copy = copy->next;
	// 	}
	// }
}

void freedoubleint(t_minishell *mini)
{
	int i;
	int size;

	i = 0;
	size = ft_commandsize(mini->commandline);
	while(i < size)
	{
		free(mini->pipes[i]);
		i++;
	}
	free(mini->pipes);
}

void heredocerror(char *str)
{
	printf("%s\n", str);
}

void heredochandle(int sig)
{
	const char  c = '\n';
    (void)sig;
    rl_replace_line("", 0);
    rl_on_new_line();
    ioctl(0, TIOCSTI, &c);
    g_sig = 130;
}

int getinput(int delflag, char *del, t_minishell *mini)
{
    int        fd[2];
    char    *line;
    char    *copy;

    pipe(fd);
    line = NULL;
	signal(SIGINT, heredochandle);
    while(1)
    {
        line = readline("> ");
		if (g_sig == 130)
		{
			close(fd[0]);
			close(fd[1]);
			callallsignals();
			setexit(130, 0);
			g_sig = 0;
			free(line);
			return (-4);
		}
        if (!line)
        {
            if (mini)
                printf("warning: here-document at line %d delimited by end-of-file (wanted '%s')\n", mini->linecount, del);
            free(line);
			break ;
        }
        copy = line;
        if (!ft_strcmp(line, del))
        {
            free(line);
            break ;
        }
        if (!delflag)
        {
            line = expand(NULL, line, mini);
            free(copy);
        }
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 2);
        free(line);
    }
	callallsignals();
    close(fd[1]);
    return (fd[0]);
}

int	openallfiles(t_minishell *mini)
{
	t_commandline *command;

	command = mini->commandline;
	while(command)
	{
		if (openfiles(&command, mini))
			return (1);
		command = command->next;
	}
	return (0);
}

void checkheredocs(t_minishell *mini)
{
	int count;
	int check;
	t_tokenize *list;

	check = 0;
	count = 0;
	list = mini->tokens;
	while(list)
	{
		if ((list->index == 0 && list->type == PIPE)
			|| (list->next && list->next->type == PIPE 
			&& list->type == PIPE) || (!list->next && list->type == PIPE))
			syntax(&check, "'|'", 0);
		else if (list->next && list->next->category && list->category)
			syntaxhere(&check, list->next->str, 0);
		else if (!list->next && list->category)
			syntax(&check, "'newline'", 0);
		else if (list->prev && list->prev->category && list->type == PIPE)
			syntax(&check, "'|'", 0);
		else if (!ft_strcmp(list->str, "\\"))
			syntax(&check, "'\\'", 0);
		else if (!ft_strcmp(list->str, ";"))
			syntax(&check, "';'", 0);
		else if (list->type == HEDOC && list->next
				&& list->next->type == DEL)
			count++;
		if (check == 1)
			break ;
		list = list->next;
	}
	if (count > 16 && check)
	{
		mini->check = 2;
		heredocerror("shell: maximum here-document count exceeded");
	}
}

void readinput(t_minishell *mini)
{
    while(1)
	{
		callallsignals();
		mini->input = readline("Minishell ➤ ");
		if (g_sig == 130)
		{
			setexit(130, 0);
			g_sig = 0;
		}
		mini->linecount++;
		if (!mini->input)
		{
			free(mini->input);
			printf("%s\n", "exit");
			exit(1);
		}
		if (mini->input[0] == '\0')
		{
			free(mini->input);
			continue;
		}
		add_history(mini->input);
		if (tokenize(mini) == -1)
		{
			freelisttokens(mini->tokens);
			free(mini->input);
			setexit(2, 0);
			continue ;
		}
		checkheredocs(mini);
		if (mini->check != 2)
			parse(mini);
		else
		{
			free(mini->input);
			freelisttokens(mini->tokens);
			exit(1);
		}
		reparse(mini);
		if (mini->check == 1)
		{
			mini->check = 0;
			setexit(2, 0);
			freelisttokens(mini->tokens);
			free(mini->input);
			continue ;
		}
		setupcommandline(mini);
		freelisttokens(mini->tokens);
		if (openallfiles(mini))
		{
			freelistcommandline(mini->commandline);
			free(mini->input);
			continue ;
		}
		execute(mini);
		freedoubleint(mini);
		freelistcommandline(mini->commandline);
		free(mini->input);
	}
}
