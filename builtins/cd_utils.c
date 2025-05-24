/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:26:51 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/23 18:24:26 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_cdhelp(char *oldcwd, int size)
{
	if (!oldcwd)
	{
		perror("getcwd");
		if (size != 1)
			exit(1);
		return (0);
	}
	return (1);
}

int	ft_cdhelp2(int size, t_commandline *command, char *oldcwd)
{
	if (command->argcount > 1)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		if (size != 1)
		{
			free(oldcwd);
			exit(1);
		}
		return (0);
	}
	return (1);
}

int	hel(char *targetdir, t_minishell *mini, int size, char *oldcwd)
{
	if (!targetdir)
	{
		targetdir = ft_getenv("HOME", &mini);
		if (!targetdir)
		{
			ft_putstr_fd("cd; HOME not set!", 2);
			if (size != 1)
			{
				free(oldcwd);
				exit(1);
			}
			return (0);
		}
	}
	return (1);
}

int	ft_cdhelp4(char *targetdir, int size, char *oldcwd)
{
	if (chdir(targetdir) == -1)
	{
		perror("cd");
		if (size != 1)
		{
			free(oldcwd);
			exit(1);
		}
		return (0);
	}
	return (1);
}

int	ft_cdhelp5(char *pwd, int size, char *oldcwd)
{
	if (!pwd)
	{
		perror("getcwd");
		if (size != 1)
		{
			free(oldcwd);
			free(pwd);
			exit(1);
		}
		return (0);
	}
	return (1);
}
