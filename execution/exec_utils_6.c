/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:10:04 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/29 14:52:08 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handleiomiddle(command);
		setup_io(command, size);
		error_check(cmd);
		handle_shlvl(cmd);
		execve(cmd->cmd, cmd->args, cmd->env);
		after_execve(cmd);
		closeheredocs(cmd->file);
		freedoubleint(mini);
		freelistcommandline(mini->commandline);
		exit(1);
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handleiolast(command);
		setup_io(command, size);
		error_check(cmd);
		handle_shlvl(cmd);
		execve(cmd->cmd, cmd->args, cmd->env);
		after_execve(cmd);
		closeheredocs(cmd->file);
		freedoubleint(mini);
		freelistcommandline(mini->commandline);
		exit(1);
	}
}
