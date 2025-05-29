/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 00:13:35 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/29 17:03:46 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handleiosinglebuiltin(t_commandline **command)
{
	int				size;
	t_commandline	*cmd;
	t_minishell		*mini;

	cmd = *command;
	mini = (*command)->mini;
	size = ft_commandsize(mini->commandline);
	if (cmd->infd == -1 || cmd->outfd == -1)
	{
		closeallpipes(mini, size);
		closeallfiles(mini);
		if (cmd->outfd > 2)
			close(cmd->outfd);
		if (cmd->infd > 2)
			close(cmd->infd);
		return (1);
	}
	if (cmd->infd == -2)
		cmd->infd = 0;
	if (cmd->outfd == -2 && cmd->next)
		cmd->outfd = mini->pipes[cmd->index][1];
	else if (cmd->outfd == -2 && !cmd->next)
		cmd->outfd = 1;
	return (0);
}

static int	is_single_builtin(t_commandline *copy, int size)
{
	return (size == 1 && checkcommand(copy->cmd));
}

static void	handle_single(t_commandline **copy, t_minishell *mini, int size)
{
	if (handleiosinglebuiltin(copy))
		return ;
	(*copy)->env = constructenv(mini->env);
	handlebuiltins(copy);
	closeallfiles(mini);
	closeallpipes(mini, size);
}

void	startpipex(t_minishell *mini)
{
	int				i;
	int				size;
	t_commandline	*copy;

	copy = mini->commandline;
	initializepipes(mini);
	size = ft_commandsize(copy);
	i = -1;
	while (++i < size && copy)
	{
		if (copy->iscmdexpand)
		{
			copy = copy->next;
			continue ;
		}
		if (is_single_builtin(copy, size))
		{
			handle_single(&copy, mini, size);
			return ;
		}
		childlabor(&copy);
		signal(SIGINT, normalhande);
		copy = copy->next;
	}
	setupchilds(mini, size);
}

void	execute(t_minishell *mini)
{
	free(mini->input);
	startpipex(mini);
}
