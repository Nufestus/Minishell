/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:48:07 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 14:32:09 by rammisse         ###   ########.fr       */
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
		while (*copy && *command && (*copy)->type != PIPE)
		{
			if ((*copy)->category)
				handlefiles(*copy, *command);
			*copy = (*copy)->next;
		}
		*command = (*command)->next;
	}
	else if ((*copy)->type == PIPE)
	{
		*copy = (*copy)->next;
		if (!*copy)
			return (0);
		while (*copy && *command && (*copy)->type != PIPE)
		{
			if ((*copy)->category)
				handlefiles(*copy, *command);
			*copy = (*copy)->next;
		}
		*command = (*command)->next;
	}
	return (1);
}

void	addfile(t_tokenize *token, t_commandline *commandline)
{
	t_tokenize		*copy;
	t_commandline	*command;

	copy = token;
	command = commandline;
	while (copy)
	{
		if (!addfilehelp(&copy, &command))
			break ;
	}
}
