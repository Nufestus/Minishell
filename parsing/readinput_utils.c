/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:48:07 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 22:05:13 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t'
		|| c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

void	countargshelp(char *args, int *i, int *count, int *in_word)
{
	if (!ft_isspace(args[*i]) && in_word == 0)
	{
		*in_word = 1;
		(*count)++;
	}
	else if (ft_isspace(args[*i]))
		*in_word = 0;
}

int	countargs(char *args, char *option)
{
	int	i;
	int	count;
	int	in_word;

	if (!args || !option)
		return (0);
	i = 0;
	count = 0;
	in_word = 0;
	while (args[i])
	{
		countargshelp(args, &i, &count, &in_word);
		i++;
	}
	i = 0;
	while (option[i])
	{
		countargshelp(option, &i, &count, &in_word);
		i++;
	}
	return (count + 1);
}

int	addfilehelp(t_tokenize **copy, t_commandline **command)
{
	if ((*copy)->index == 0)
	{
		while (*copy && (*copy)->type != PIPE)
		{
			if ((*copy)->category)
				if (handlefiles(*copy, *command))
					return (0);
			*copy = (*copy)->next;
		}
	}
	else if ((*copy)->type == PIPE)
	{
		*copy = (*copy)->next;
		while (*copy && (*copy)->type != PIPE)
		{
			if ((*copy)->category)
				if (handlefiles(*copy, *command))
					return (0);
			*copy = (*copy)->next;
		}
	}
	return (1);
}

int	addfile(t_tokenize *token, t_commandline *commandline)
{
	t_tokenize		*copy;
	t_commandline	*command;

	copy = token;
	command = commandline;
	while (copy && command)
	{
		if (!addfilehelp(&copy, &command))
			return (1);
		command = command->next;
	}
	return (0);
}
