/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:50:52 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 01:47:28 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	isnotnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	exithelp(int flag, t_commandline *command)
{
	if (flag)
	{
		printf("exit: %s: numeric argument required\n", command->args[1]);
		return (0);
	}
	return (1);
}

int	exithelp2(t_commandline *command)
{
	int	size;

	size = ft_commandsize(command->mini->commandline);
	if (command->args[1] && command->args[2])
	{
		printf("exit: too many arguments\n");
		setexit(1, 0);
		if (size > 1)
			exit(1);
		return (0);
	}
	return (1);
}

void	ft_exit(t_commandline *command)
{
	int			size;
	long long	status;
	int			flag;

	status = setexit(0, 1);
	size = ft_commandsize(command->mini->commandline);
	flag = 0;
	if (size == 1)
		printf("exit\n");
	if (!exithelp2(command))
		return ;
	if (command->args[1] && isnotnum(command->args[1]))
	{
		status = ft_atoi(command->args[1], &flag);
		if (!exithelp(flag, command))
			exit(2);
	}
	else if ((command->args[1] && !isnotnum(command->args[1])) || flag)
	{
		printf("exit: %s: numeric argument required\n", command->args[1]);
		exit(2);
	}
	freelistenv(command->mini->env);
	freelistcommandline(command->mini->commandline);
	exit(status);
}
