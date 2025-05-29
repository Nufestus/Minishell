/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:26:51 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/29 12:13:17 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_cdhelp(char *oldcwd, int size, t_commandline *command, t_minishell *mini)
{
	if (!oldcwd)
	{
		perror("cd: error retrieving current directory: getcwd: \
cannot access parent directories");
		if (!hel(&command->args[1], mini, size, oldcwd))
			return (0);
		if (!ft_cdhelp4(command->args[1], size, oldcwd))
			return (0);
		if (my_getenv(mini, "PWD"))
			mini->pwd = my_getenv(mini, "PWD");
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
		free(oldcwd);
		return (0);
	}
	return (1);
}

int	hel(char **targetdir, t_minishell *mini, int size, char *oldcwd)
{
	if (!*targetdir)
	{
		*targetdir = ft_strdup(ft_getenv("HOME", mini));
		if (!*targetdir)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			if (size != 1)
			{
				if (oldcwd)
					free(oldcwd);
				exit(1);
			}
			free(oldcwd);
			return (0);
		}
	}
	return (1);
}

int	ft_cdhelp4(char *targetdir, int size, char *oldcwd)
{
	if (chdir(targetdir) == -1)
	{
		perror(targetdir);
		if (size != 1)
		{
			if (oldcwd)
				free(oldcwd);
			exit(1);
		}
		free(oldcwd);
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
		free(oldcwd);
		return (0);
	}
	return (1);
}
