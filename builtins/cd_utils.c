/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:26:51 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/31 17:28:05 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_cdhelp(char *oldcwd, int size, t_commandline *command, t_minishell *mini)
{
	char *copy;
	char *string;

	string = ft_strdup(command->args[1]);
	if (!oldcwd)
	{
		if (mini->pwd)
			free(mini->pwd);
		perror("cd: error retrieving current directory: getcwd: \
cannot access parent directories");
		if (!hel(&string, mini, size, oldcwd))
			return (free(string), 0);
		if (!ft_cdhelp4(string, size, oldcwd))
			return (free(string), 0);
		if (my_getenv(mini, "PWD"))
		{
			mini->pwd = my_getenv(mini, "PWD");
			mini->pwd = ft_strjoin(mini->pwd, "/");
			copy = mini->pwd;
			mini->pwd = ft_strjoin(mini->pwd, string);
			free(copy);
		}
		free(string);
		if (size != 1)
			exit(1);
		setexit(1, 0);
		return (0);
	}
	free(string);
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
		setexit(1, 0);
		return (0);
	}
	return (1);
}

int	hel(char **targetdir, t_minishell *mini, int size, char *oldcwd)
{
	if (*targetdir && *targetdir[0] == '\0')
		return (1);
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
			setexit(1, 0);
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
			free(oldcwd);
			free(targetdir);
			exit(1);
		}
		free(oldcwd);
		free(targetdir);
		setexit(1, 0);
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
