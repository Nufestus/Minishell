/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:46:10 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/01 20:00:06 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handleforksingle(t_commandline **command, pid_t pid, int size)
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
		handleiosingle(command);
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

void	setupfirstcommand(t_commandline ***command)
{
	int				size;
	t_commandline	*cmd;
	t_minishell		*mini;

	cmd = **command;
	mini = cmd->mini;
	size = ft_commandsize(mini->commandline);
	cmd->env = constructenv(mini->env);
	cmd->pid = fork();
	handleforksingle(*command, cmd->pid, size);
}

void	childlabor(t_commandline **command)
{
	t_commandline	*cmd;

	cmd = *command;
	if (!cmd->index)
		setupfirstcommand(&command);
	else if (!cmd->next)
		setuplastcommand(&command);
	else
		setupmiddlecommand(&command);
}

void	closeallpipes(t_minishell *mini, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		close(mini->pipes[i][0]);
		close(mini->pipes[i][1]);
		i++;
	}
}

void	setupchilds(t_minishell *mini, int size)
{
	t_setupchild	child;

	initializechild(&child, size, mini);
	while (child.copy)
	{
		if (child.copy->pid)
		{
			waitpid(child.copy->pid, &child.status, 0);
			if (WIFSIGNALED(child.status))
			{
				child.sig = WTERMSIG(child.status);
				handlesig(&child);
			}
		}
		child.copy = child.copy->next;
	}
	if (child.skip != 1)
		setexit(WEXITSTATUS(child.status), 0);
	closeallfiles(mini);
}
