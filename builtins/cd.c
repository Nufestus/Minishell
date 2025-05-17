/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 22:52:01 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/17 20:35:20 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_setenv(char *envname, char *newvalue, t_minishell *mini)
{
	t_env	*env;
	char	*fullstring;
	char	*copy;

	env = mini->env;
	while (env)
	{
		if (!ft_strcmp(env->variable, envname))
		{
			if (env->value)
				free(env->value);
			env->value = ft_strdup(newvalue);
			fullstring = ft_strjoin(env->variable, "=");
			copy = fullstring;
			fullstring = ft_strjoin(fullstring, env->value);
			free(copy);
			free(env->string);
			env->string = ft_strdup(fullstring);
			free(fullstring);
			return ;
		}
		env = env->next;
	}
}

void	cdextension(t_commandline *command, char *oldcwd, int size)
{
	if (!oldcwd)
	{
		perror("getcwd");
		if (size != 1)
			exit(1);
		return ;
	}
	if (command->argcount > 1)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		if (size != 1)
		{
			free(oldcwd);
			exit(1);
		}
		return ;
	}
}

void	normhelp(char *targetdir, int size, t_minishell *mini, char *oldcwd)
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
			return ;
		}
	}
	if (chdir(targetdir) == -1)
	{
		perror("cd");
		if (size != 1)
		{
			free(oldcwd);
			exit(1);
		}
		return ;
	}
}

void	checkpwd(char *pwd, int size, char *oldcwd)
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
		return ;
	}
}

void	ft_cd(t_commandline *commandline)
{
	int			size;
	char		*targetdir;
	t_minishell	*mini;
	char		*oldcwd;
	char		*pwd;

	size = ft_commandsize(commandline->mini->commandline);
	mini = commandline->mini;
	targetdir = commandline->args[1];
	oldcwd = getcwd(0, 0);
	cdextension(commandline, oldcwd, size);
	normhelp(targetdir, size, mini, oldcwd);
	pwd = getcwd(0, 0);
	checkpwd(pwd, size, oldcwd);
	ft_setenv("PWD", pwd, mini);
	ft_setenv("OLDPWD", oldcwd, mini);
	free(oldcwd);
	free(pwd);
	if (size != 1)
		exit(0);
	commandline->mini->exitstatus = 0;
	return ;
}
