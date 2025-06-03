/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:44:19 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/01 18:23:47 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	printerror(char *str)
{
	write(2, str, ft_strlen(str));
	write(2, ": Permission denied\n", 21);
}

void	error(char *str)
{
	char	*tmp;

	tmp = ft_strjoin(str, ": command not found\n");
	write(2, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

void	handlebuiltins(t_commandline **command)
{
	if ((*command)->cmd)
	{
		if (!ft_strcmp((*command)->cmd, "env"))
			ft_env((*command)->mini, (*command)->args);
		else if (!ft_strcmp((*command)->cmd, "pwd"))
			ft_pwd((*command)->mini, *command);
		else if (!ft_strcmp((*command)->cmd, "cd"))
			ft_cd((*command));
		else if (!ft_strcmp((*command)->cmd, "echo"))
			ft_echo((*command));
		else if (!ft_strcmp((*command)->cmd, "unset"))
			unset((*command));
		else if (!ft_strcmp((*command)->cmd, "exit"))
			ft_exit((*command));
		else if (!ft_strcmp((*command)->cmd, "export"))
			export((*command));
	}
}

void	closeallfiles(t_minishell *mini)
{
	t_commandline	*copy;

	copy = mini->commandline;
	while (copy)
	{
		if (copy->outfd > 2)
			close(copy->outfd);
		if (copy->infd > 2)
			close(copy->infd);
		copy = copy->next;
	}
}

void	handleiolast(t_commandline **command)
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
		safe_exit(1);
	}
	if (cmd->infd == -2)
		cmd->infd = mini->pipes[cmd->index - 1][0];
	if (cmd->outfd == -2)
		cmd->outfd = 1;
}
