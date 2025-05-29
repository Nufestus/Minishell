/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:45:03 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/29 14:20:41 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setuplastcommand(t_commandline ***command)
{
	int				size;
	t_commandline	*cmd;
	t_minishell		*mini;

	cmd = **command;
	mini = cmd->mini;
	size = ft_commandsize(mini->commandline);
	cmd->env = constructenv(mini->env);
	cmd->pid = fork();
	handleforklast(*command, cmd->pid, size);
}

void	handleiomiddle(t_commandline **command)
{
	int				size;
	t_commandline	*cmd;
	t_minishell		*mini;

	cmd = *command;
	mini = cmd->mini;
	size = ft_commandsize(mini->commandline);
	if (cmd->infd == -1 || cmd->outfd == -1)
	{
		closeallpipes(mini, size);
		closeallfiles(mini);
		freedoubleint(mini);
		if (cmd->outfd > 2)
			close(cmd->outfd);
		if (cmd->infd > 2)
			close(cmd->infd);
		exit(1);
	}
	if (cmd->infd == -2)
		cmd->infd = mini->pipes[cmd->index - 1][0];
	if (cmd->outfd == -2)
		cmd->outfd = mini->pipes[cmd->index][1];
}

void	setupmiddlecommand(t_commandline ***command)
{
	int				size;
	t_commandline	*cmd;
	t_minishell		*mini;

	cmd = **command;
	mini = cmd->mini;
	size = ft_commandsize(mini->commandline);
	cmd->env = constructenv(mini->env);
	cmd->pid = fork();
	handleforkmiddle(*command, cmd->pid, size);
}

int	checkcommand(char *cmd)
{
	if (cmd)
	{
		if (!ft_strcmp(cmd, "cd")
			|| !ft_strcmp(cmd, "env")
			|| !ft_strcmp(cmd, "pwd")
			|| !ft_strcmp(cmd, "echo")
			|| !ft_strcmp(cmd, "export")
			|| !ft_strcmp(cmd, "unset")
			|| !ft_strcmp(cmd, "exit"))
			return (1);
	}
	return (0);
}

int	ft_atoi_custom(const char *str)
{
	int			sign;
	int			i;
	long long	result;
	long long	old_result;

	result = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		old_result = result;
		result = result * 10 + (sign * (str[i] - '0'));
		if (result > INT_MAX || result < INT_MIN)
			return (1);
		i++;
	}
	return (result + 1);
}
