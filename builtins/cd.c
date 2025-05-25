/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 22:52:01 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/25 01:07:33 by rammisse         ###   ########.fr       */
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
	if (!ft_cdhelp(oldcwd, size))
		return ;
	if (!ft_cdhelp2(size, commandline, oldcwd))
		return ;
	if (!hel(commandline->args[1], mini, size, oldcwd))
		return ;
	if (!ft_cdhelp4(commandline->args[1], size, oldcwd))
		return ;
	pwd = getcwd(0, 0);
	if (!ft_cdhelp5(pwd, size, oldcwd))
		return ;
	ft_setenv("PWD", pwd, mini);
	ft_setenv("OLDPWD", oldcwd, mini);
	ft_cdhelp6(size, pwd, oldcwd);
	setexit(0, 0);
	return ;
}
