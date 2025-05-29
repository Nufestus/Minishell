/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 22:52:01 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/27 19:52:08 by aammisse         ###   ########.fr       */
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

void	ft_cdhelp6(int size, char *pwd, char *oldcwd)
{
	free(oldcwd);
	free(pwd);
	if (size != 1)
		exit(0);
	return ;
}

void	add_to_env(char *str, char *s, t_minishell *mini)
{
	char	*copy;
	char	*full;
	char	*string;
	t_env	*new;

	string = ft_getenv(str, mini);
	if (!string)
	{
		full = ft_strjoin(str, "=");
		copy = full;
		full = ft_strjoin(full, s);
		free(copy);
		new = ft_envnew(s, str, full);
		ft_envadd_back(&mini->env, new);
		free(full);
	}
	else
		ft_setenv(str, s, mini);
}

int	ft_cdhelper(int size, char *oldcwd, t_minishell *mini, char *pwd)
{
	if (!ft_cdhelp5(pwd, size, oldcwd))
		return (0);
	add_to_env("PWD", pwd, mini);
	add_to_env("OLDPWD", oldcwd, mini);
	ft_cdhelp6(size, pwd, oldcwd);
	setexit(0, 0);
	return (1);
}

void	ft_cd(t_commandline *commandline)
{
	int				size;
	t_minishell		*mini;
	char			*oldcwd;
	char			*pwd;

	size = ft_commandsize(commandline->mini->commandline);
	mini = commandline->mini;
	oldcwd = getcwd(0, 0);
	pwd = NULL;
	if (!ft_cdhelp(oldcwd, size, commandline, mini))
		return ;
	if (!ft_cdhelp2(size, commandline, oldcwd))
		return ;
	if (commandline->args[1])
		pwd = ft_strdup(commandline->args[1]);
	if (!hel(&pwd, mini, size, oldcwd))
		return ;
	if (!ft_cdhelp4(pwd, size, oldcwd))
		return ;
	free(pwd);
	pwd = getcwd(0, 0);
	if (!ft_cdhelper(size, oldcwd, mini, pwd))
		return ;
	return ;
}
