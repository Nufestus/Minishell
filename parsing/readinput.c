/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:06:43 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/19 22:47:51 by aammisse         ###   ########.fr       */
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

void	setupcommandline(t_minishell *mini)
{
	char *cmd;
	int i;
	char *arg;
	char *option;
	t_commandline *command;
	t_commandline *copy;
	t_tokenize *token;

	token = mini->tokens;
	command = NULL;
	copy = NULL;
	while(token)
	{
		if (token->type == PIPE)
		{
			cmd = NULL;
			arg = NULL;
			option = NULL;
			token = token->next;
			while (token && token->type != PIPE)
			{
				if (token->type == CMD)
					cmd = token->str;
				else if (token->type == ARG)
					arg = token->str;
				else if (token->type == OPTION)
					option = token->str;
				// add here the outfile / infile linked list handling
				token = token->next;
			}
			command = ft_commandnew(cmd, option, arg, countargs(arg, option));
			command->numargs = countargs(arg, option) + 1;
			ft_commandadd_back(mini->commandline, command);
		}
		if (token)
			token = token->next;
	}
	copy = mini->commandline;
	while(copy)
	{
		i = 0;
		printf("Cmd: %s\nArgs: ", copy->cmd);
		while(copy->args[i])
			printf("%s , ", copy->args[i++]);
		printf("\n Num of Args: %d\n", copy->numargs);
		copy = copy->next;
	}
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
		tokenize(mini);
		parse(mini);
		setupcommandline(mini);
	}
}