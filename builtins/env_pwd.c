/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:02:28 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/01 20:33:49 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwdhelp(int size, char *path, t_minishell *mini)
{
	if (size > 1)
	{
		if (path)
			printf("%s\n", path);
		else
			perror("pwd");
	}
	else
	{
		if (path)
		{
			ft_putstr_fd(path, mini->commandline->outfd);
			ft_putstr_fd("\n", mini->commandline->outfd);
		}
		else
			perror("pwd");
	}
}

int	anoption(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		if (str[i])
			return (1);
	}
	return (0);
}

void	ft_pwd(t_minishell *mini, t_commandline *command)
{
	char	*path;
	int		size;

	path = NULL;
	size = ft_commandsize(mini->commandline);
	if (command->args[1] && anoption(command->args[1]))
	{
		write(2, "pwd: invalid option\n", 21);
		if (size != 1)
			exit(2);
		setexit(2, 0);
		return ;
	}
	path = getcwd(NULL, 0);
	if (!path)
		path = ft_strdup(my_getenv(mini, "PWD"));
	ft_pwdhelp(size, path, mini);
	if (path)
		free(path);
	if (size != 1)
		exit(0);
	setexit(0, 0);
}

void	ft_envhelp(int size, t_env *env, t_minishell *mini)
{
	if (size > 1)
	{
		while (env)
		{
			if (env->value)
				printf("%s\n", env->string);
			env = env->next;
		}
	}
	else
	{
		while (env)
		{
			if (env->value)
			{
				ft_putstr_fd(env->string, mini->commandline->outfd);
				ft_putstr_fd("\n", mini->commandline->outfd);
			}
			env = env->next;
		}
	}
}

void	ft_env(t_minishell *mini, char **args)
{
	t_env	*env;
	int		size;

	size = ft_commandsize(mini->commandline);
	if (args[1])
	{
		if (anoption(args[1]))
		{
			ft_putstr_fd("env: invalid option\n", STDERR_FILENO);
			if (size != 1)
				safe_exit(125);
			setexit(125, 0);
			return ;
		}
		ft_putstr_fd("env: no such file or directory\n", STDERR_FILENO);
		if (size != 1)
			safe_exit(127);
		setexit(127, 0);
		return ;
	}
	env = mini->env;
	ft_envhelp(size, env, mini);
	envhelp(size, mini);
	return ;
}
