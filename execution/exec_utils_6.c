/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:10:04 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/02 19:03:50 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigdfl(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	handleforkmiddle(t_commandline **command, pid_t pid, int size)
{
	t_minishell		*mini;
	t_commandline	*cmd;

	cmd = *command;
	mini = cmd->mini;
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (!pid)
	{
		sigdfl();
		clear_history();
		handleiomiddle(command);
		setup_io(command, size);
		error_check(cmd);
		handle_shlvl(cmd);
		execve(cmd->cmd, cmd->args, cmd->env);
		after_execve(cmd);
		closeheredocs(cmd->file);
		freedoubleint(mini);
		freelistcommandline(mini->commandline);
		safe_exit(1);
	}
}

void	handleforklast(t_commandline **command, pid_t pid, int size)
{
	t_minishell		*mini;
	t_commandline	*cmd;

	cmd = *command;
	mini = cmd->mini;
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (!pid)
	{
		sigdfl();
		clear_history();
		handleiolast(command);
		setup_io(command, size);
		error_check(cmd);
		handle_shlvl(cmd);
		execve(cmd->cmd, cmd->args, cmd->env);
		after_execve(cmd);
		closeheredocs(cmd->file);
		freedoubleint(mini);
		freelistcommandline(mini->commandline);
		safe_exit(1);
	}
}

void	handlesig(t_setupchild *child)
{
	if (child->sig == SIGINT)
	{
		setexit(130, 0);
		if (!child->copy->next)
			child->skip = 1;
	}
	else if (child->sig == SIGQUIT)
	{
		setexit(131, 0);
		if (!child->copy->next)
			child->skip = 1;
	}
}
