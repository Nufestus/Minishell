/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:46:10 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/24 22:58:38 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setupfirstcommand(t_commandline ***command)
{
	int			size;
	pid_t		pid;
	t_commandline *cmd;
	t_minishell	*mini;

	cmd = **command;
	mini = cmd->mini;
	size = ft_commandsize(mini->commandline);
	cmd->env = constructenv(mini->env);
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handleiosingle(*command);
		setup_io(cmd, size);
		error_check(cmd);
		handle_shlvl(cmd);
		execve(cmd->cmd, cmd->args, cmd->env);
		after_execve(cmd);
		free(cmd->cmd);
		freedoublearray(cmd->args);
		exit(1);
	}
}

void	childlabor(t_commandline **command)
{
	t_commandline *cmd;

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
	int				status;
	t_commandline	*copy;

	closeallpipes(mini, size);
	while (waitpid(-1, &status, 0) != -1)
		;
	copy = mini->commandline;
	while (copy)
	{
		if (copy->outfd != -1 && copy->outfd != -2)
			close(copy->outfd);
		if (copy->infd != -1 && copy->infd != -2 && copy->infd != -4)
			close(copy->infd);
		copy = copy->next;
	}
	setexit(WEXITSTATUS(status), 0);
}

// int	command_loop(t_commandline **copy, t_minishell *mini, int size)
// {
// 	if (size > 1 || !checkcommand((*copy)->cmd))
// 	{
// 		if (mini->commandline->infd == -4)
// 		{
// 			(*copy) = (*copy)->next;
// 			return (1);
// 		}
// 		childlabor(copy);
// 	}
// 	else if (size == 1)
// 	{
// 		if (mini->commandline->infd == -1)
// 		{
// 			(*copy) = (*copy)->next;
// 			return (1);
// 		}
// 		handleiosingle(&mini->commandline);
// 		closeallpipes(mini, size);
// 		mini->commandline->env = constructenv(mini->env);
// 		handlebuiltins((*copy));
// 		return (0);
// 	}
// 	return (-1);
// }
