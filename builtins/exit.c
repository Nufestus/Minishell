/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:50:52 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/24 22:57:58 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	atoihelp(int *overflow, long long result, long long old_result)
{
	if (old_result != result / 10)
	{
		*overflow = 1;
		return (0);
	}
	return (1);
}

long long	ft_atoi(char *str, int *overflow)
{
	int			i;
	int			sign;
	long long	old_result;
	long long	result;

	sign = 1;
	i = 0;
	result = 0;
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
		result = result * 10 + str[i] - '0';
		if (!atoihelp(overflow, result, old_result))
			return (0);
		i++;
	}
	return (result * sign);
}

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

void	ft_exit(t_commandline *command)
{
	long long	status;
	int			flag;

	status = setexit(0, 1);;
	flag = 0;
	printf("exit\n");
	if (command->args[1] && command->args[2])
	{
		printf("exit: too many arguments\n");
		return ;
	}
	if (command->args[1] && isnotnum(command->args[1]))
	{
		status = ft_atoi(command->args[1], &flag);
		if (!exithelp(flag, command))
			return ;
	}
	else if ((command->args[1] && !isnotnum(command->args[1])) || flag)
	{
		printf("exit: %s: numeric argument required\n", command->args[1]);
		return ;
	}
	freelistenv(command->mini->env);
	freelistcommandline(command->mini->commandline);
	exit(status);
}
