/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:59:54 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 15:07:34 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	getarguments(t_tokenize *tokens)
{
	int			res;
	t_tokenize	*token;

	res = 0;
	token = tokens;
	while (token && token->type != PIPE)
	{
		res++;
		token = token->next;
	}
	return (res);
}

void	freedoubleint(t_minishell *mini)
{
	int	i;
	int	size;

	i = 0;
	size = ft_commandsize(mini->commandline);
	while (i < size)
	{
		free(mini->pipes[i]);
		i++;
	}
	free(mini->pipes);
}

void	heredochandle(int sig)
{
	const char	c = '\n';

	(void)sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	ioctl(0, TIOCSTI, &c);
	g_sig = 130;
}

int	openallfiles(t_minishell *mini)
{
	t_commandline	*command;

	command = mini->commandline;
	while (command)
	{
		if (openfiles(&command, mini))
			return (1);
		command = command->next;
	}
	return (0);
}
