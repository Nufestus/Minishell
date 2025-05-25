/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:16:02 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/25 01:46:32 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle(char *str, char *str1)
{
	char	*string;

	if (str1 != NULL && str == NULL)
	{
		string = malloc(ft_strlen(str1) + 1);
		if (!string)
			return (NULL);
		ft_strlcpy(string, str1, ft_strlen(str1) + 1);
		return (string);
	}
	else if (str != NULL && str1 == NULL)
	{
		string = malloc(ft_strlen(str) + 1);
		if (!string)
			return (NULL);
		ft_strlcpy(string, str, ft_strlen(str) + 1);
		return (string);
	}
	else
		return (NULL);
}

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
	if (size > 1)
	{
		if (optioncheck)
			printf("%s\n", res);
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
}

void	initechovars(int *i, int *optioncheck, int *finishedoptions, char **res)
{
	*i = 1;
	*optioncheck = 0;
	*finishedoptions = 0;
	*res = NULL;
}

void	ft_echo(t_commandline *command)
{
	t_echo	echo;

	initechovars(&echo.i, &echo.optioncheck, &echo.finishedoptions, &echo.res);
	echo.size = ft_commandsize(command->mini->commandline);
	echo.copy = NULL;
	echo_loop(command, &echo);
	helpecho(echo.size, echo.optioncheck, echo.res, command);
	free(echo.copy);
	if (echo.size > 1)
		exit(0);
	setexit(0, 0);
	return ;
}
