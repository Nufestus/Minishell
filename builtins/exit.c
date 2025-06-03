/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:50:52 by rammisse          #+#    #+#             */
/*   Updated: 2025/06/03 13:59:28 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*no_file(char *str, t_commandline *command)
{
	char	*path;
	char	*copy;
	char	*res;

	copy = NULL;
	if (!access(str, F_OK | X_OK) && !is_directory(str))
	{
		res = ft_strdup(str);
		path = getcwd(NULL, 0);
		copy = path;
		path = ft_strjoin(path, "/");
		free(copy);
		copy = res;
		res = ft_strjoin(path, res);
		return (free(copy), free(path), res);
	}
	if ((is_directory(str) && ft_find(str, "/"))
		|| (!ft_getenv("PATH", command->mini) && !ft_find(str, "/")))
		handlesuchfile(str, command);
	return (NULL);
}

int	isnotnum(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
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
	if (command->args[1] && command->args[2] && isnotnum(command->args[1]))
	{
		printf("exit: too many arguments\n");
		setexit(1, 0);
		if (size > 1)
			safe_exit(1);
		return (0);
	}
	return (1);
}

void	ft_exit(t_commandline *command)
{
	int			size;
	long long	status;
	int			flag;

	size = ft_commandsize(command->mini->commandline);
	flag = 0;
	status = setexit(0, 1);
	if (size == 1)
		printf("exit\n");
	if (!exithelp2(command))
		return ;
	if (command->args[1] && isnotnum(command->args[1]))
	{
		status = ft_atoi(command->args[1], &flag);
		if (!exithelp(flag, command))
			safe_exit(2);
	}
	else if ((command->args[1] && !isnotnum(command->args[1])) || flag)
	{
		printf("exit: %s: numeric argument required\n", command->args[1]);
		safe_exit(2);
	}
	freelistenv(command->mini->env);
	freelistcommandline(command->mini->commandline);
	safe_exit(status);
}
