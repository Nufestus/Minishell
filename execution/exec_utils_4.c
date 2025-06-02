/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:45:35 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/02 11:04:04 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handleiosingle(t_commandline **command)
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
		freedoubleint(mini);
		if (cmd->outfd > 2)
			close(cmd->outfd);
		if (cmd->infd > 2)
			close(cmd->infd);
		freelistcommandline(mini->commandline);
		safe_exit(1);
	}
	if (cmd->infd == -2)
		cmd->infd = 0;
	if (cmd->outfd == -2 && cmd->next)
		cmd->outfd = mini->pipes[cmd->index][1];
	else if (cmd->outfd == -2 && !cmd->next)
		cmd->outfd = 1;
}

void	setup_io(t_commandline **command, int size)
{
	if ((*command)->infd != STDIN_FILENO)
	{
		dup2((*command)->infd, STDIN_FILENO);
		close((*command)->infd);
	}
	if ((*command)->outfd != STDOUT_FILENO)
	{
		dup2((*command)->outfd, STDOUT_FILENO);
		close((*command)->outfd);
	}
	closeallfiles((*command)->mini);
	closeallpipes((*command)->mini, size);
	if ((*command)->cmd && (*command)->cmd[0] == '\0')
	{
		error("\'\'");
		freedoubleint((*command)->mini);
		freelistcommandline((*command)->mini->commandline);
		safe_exit(127);
	}
	else if (!(*command)->cmd)
	{
		freedoubleint((*command)->mini);
		freelistcommandline((*command)->mini->commandline);
		safe_exit(0);
	}
}

void	error_check(t_commandline *command)
{
	if (!ft_strcmp(command->args[0], ".")
		|| (!ft_strncmp(command->args[0], "..", 2)
			&& !ft_strchr(command->args[0], '/')))
		handlepoint(command);
	if (!ft_find(command->args[0], "/") && !ft_find(command->args[0], ".")
		&& !checkcommand(command->cmd))
	{
		free(command->cmd);
		command->cmd = checkfile(command);
	}
	if (!command->cmd)
		command->cmd = no_file(command->args[0], command);
	else if (access(command->cmd, F_OK) == 0 && access(command->cmd, X_OK) == -1
		&& !is_directory(command->cmd))
	{
		printerror(command->cmd);
		freedoubleint(command->mini);
		freelistcommandline(command->mini->commandline);
		safe_exit(126);
	}
	else if ((is_directory(command->cmd)) && ((ft_find(command->args[0], ".")
				&& ft_find(command->args[0], "/"))
			|| ft_find(command->args[0], "/")))
		directory_free(command);
	handlebuiltins(&command);
}

void	handle_shlvl(t_commandline *command)
{
	int		num;
	char	*tmp;
	char	*value;
	t_env	*node;

	if (my_getenv(command->mini, "SHLVL")
		&& !ft_strcmp(command->args[0], "./minishell"))
	{
		node = getenvnode(command->mini->env, "SHLVL");
		num = ft_atoi_custom(node->value);
		value = ft_itoa(num);
		free(node->value);
		node->value = ft_strdup(value);
		node->string = ft_strjoin(node->variable, "=");
		tmp = node->string;
		node->string = ft_strjoin(node->string, node->value);
		free(tmp);
		freedoublearray(command->env);
		command->env = constructenv(command->mini->env);
	}
}

void	after_execve(t_commandline *command)
{
	t_minishell	*mini;

	mini = command->mini;
	if (command->args && !ft_find(command->args[0], "/")
		&& !ft_find(command->args[0], "."))
	{
		error(command->args[0]);
		freedoubleint(mini);
		freelistcommandline(mini->commandline);
	}
	else if (command->args && (access(command->args[0], F_OK)))
	{
		write(2, command->args[0], ft_strlen(command->args[0]));
		write(2, ": No such file or directory\n", 29);
		freedoubleint(mini);
		freelistcommandline(mini->commandline);
	}
	safe_exit(127);
}
