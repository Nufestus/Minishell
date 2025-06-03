/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:16:02 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/02 11:21:06 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fillfirst(const char *s1, char *join, int i)
{
	while (s1[i] != '\0')
	{
		join[i] = s1[i];
		i++;
	}
	join[i] = 32;
	i++;
}

void	helpecho(int size, int optioncheck, char *res, t_commandline *command)
{
	int	check;

	checkset(&res, &check);
	if (size > 1)
	{
		if (optioncheck)
			printf("%s", res);
		else
			printf("%s\n", res);
	}
	else
	{
		if (optioncheck)
			ft_putstr_fd(res, command->outfd);
		else
		{
			ft_putstr_fd(res, command->outfd);
			ft_putstr_fd("\n", command->outfd);
		}
	}
	if (check)
		free(res);
}

void	initechovars(int *i, int *optioncheck, int *finishedoptions, char **res)
{
	*i = 1;
	*optioncheck = 0;
	*finishedoptions = 0;
	*res = NULL;
}

void	safe_exit(int status)
{
	clear_history();
	exit(status);
}

void	ft_echo(t_commandline *command)
{
	t_echo	echo;

	initechovars(&echo.i, &echo.optioncheck, &echo.finishedoptions, &echo.res);
	echo.size = ft_commandsize(command->mini->commandline);
	echo.copy = NULL;
	echo_loop(command, &echo);
	helpecho(echo.size, echo.optioncheck, echo.res, command);
	free(echo.res);
	if (echo.size > 1)
		safe_exit(0);
	setexit(0, 0);
	return ;
}
