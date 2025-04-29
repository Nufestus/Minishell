/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:06:43 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/28 21:23:07 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	char *file;
	char *del;

	type = 0;
	file = NULL;
	del = NULL;
	new = NULL;
	list = NULL;
	if (token->type == REDIN || token->type == APPEND
			|| token->type == REDOUT)
	{
		type = token->type;
		file = ft_strdup(token->next->str);
	}
	else if (token->type == HEDOC)
	{
		type = token->type;
		del = ft_strdup(token->next->str);
	}
	new = ft_filenew(file, del, type);
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

void	setupcommandline(t_minishell *mini)
{
	char *cmd;
	int index;
	char *arg;
	char *option;
	char *cool;
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
		cmd = NULL;
		arg = NULL;
		option = NULL;
		if (token->index == 0)
		{
			while (token && token->type != PIPE)
			{
				if (token->type == CMD)
					cmd = token->str;
				else if (token->type == ARG)
				{
					cool = arg;
					arg = ft_strjoin(arg, " ");
					free(cool);
					cool = arg;
					arg = ft_strjoin(arg, token->str);
					free(cool);
				}
				else if (token->type == OPTION)
					option = token->str;
				token = token->next;
			}
			if (!cmd)
				cmd = ft_strdup("NONE");
			command = ft_commandnew(cmd, option, arg);
			command->index = index;
			command->mini = mini;
			ft_commandadd_back(&mini->commandline, command);  
			free(arg);
			if (!ft_strcmp(cmd, "NONE"))
				free(cmd);
			index++;
		}
		else if (token->type == PIPE)
		{
			token = token->next;
			if (!token)
				break ;
			while (token && token->type != PIPE)
			{
				if (token->type == CMD)
					cmd = token->str;
				else if (token->type == ARG)
				{
					cool = arg;
					arg = ft_strjoin(arg, " ");
					free(cool);
					cool = arg;
					arg = ft_strjoin(arg, token->str);
					free(cool);
				}
				else if (token->type == OPTION)
					option = token->str;
				token = token->next;
			}
			if (!cmd)
				cmd = ft_strdup("NONE");
			command = ft_commandnew(cmd, option, arg);
			command->index = index;
			command->mini = mini;
			ft_commandadd_back(&mini->commandline, command);
			free(arg);
			if (!ft_strcmp(cmd, "NONE"))
				free(cmd);
			index++;
		}
	}
	addfile(file, mini->commandline);
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
	// 		printf("Cmd: %s\nArgs: ", copy->cmd);
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

void readinput(t_minishell *mini)
{
    while(1)
	{
		mini->input = readline(INPUT1 INPUT2);
		if (!mini->input)
		{
			free(mini->input);
			break ;
		}
		add_history(mini->input);
		if (tokenize(mini) == -1)
		{
			freelisttokens(mini->tokens);
			continue ;
		}
		parse(mini);
		if (mini->check == 1)
		{
			mini->check = 0;
			freelisttokens(mini->tokens);
			continue;
		}
		setupcommandline(mini);
		execute(mini);
		freedoubleint(mini);
		freelisttokens(mini->tokens);
		freelistcommandline(mini->commandline);
	}
}
