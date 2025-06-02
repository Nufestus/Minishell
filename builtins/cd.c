/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 18:22:22 by rammisse          #+#    #+#             */
/*   Updated: 2025/06/02 11:26:12 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	change_dir(char *dir, int size)
{
	if (anoption(dir))
	{
		write(2, "cd: invalid option\n", 20);
		if (size > 1)
			safe_exit(2);
		return (setexit(2, 0), 1);
	}
	if (chdir(dir) == -1)
	{
		perror(dir);
		if (size != 1)
			safe_exit(1);
		setexit(1, 0);
		return (1);
	}
	return (0);
}

int	handle_error(t_commandline *command, int size)
{
	if (!command->args[1])
	{
		write(2, "cd: invalid arguments\n", 23);
		if (size > 1)
			safe_exit(1);
		return (setexit(1, 0), 1);
	}
	else if (command->args[1] && command->args[1][0] == '\0'
		&& command->argcount == 1)
	{
		if (size > 1)
			safe_exit(0);
		return (setexit(0, 0), 1);
	}
	else if (command->argcount > 1)
	{
		write(2, "cd: too many arguments\n", 24);
		if (size > 1)
			safe_exit(1);
		return (setexit(1, 0), 1);
	}
	return (0);
}

void	go_to_root(int size)
{
	if (chdir("/") == -1)
	{
		perror("/");
		if (size != 1)
			safe_exit(1);
		setexit(1, 0);
		return ;
	}
	return ;
}

int	cd_init(t_commandline *command, int size, char **oldpwd, t_minishell *mini)
{
	if (handle_error(command, size))
		return (1);
	if (my_getenv(mini, "PWD"))
		*oldpwd = ft_strdup(my_getenv(mini, "PWD"));
	if (change_dir(command->args[1], size))
		return (free(*oldpwd), 1);
	return (0);
}

int	ft_cd(t_commandline *command)
{
	int			size;
	char		*pwd;
	char		*oldpwd;
	t_minishell	*mini;

	mini = command->mini;
	pwd = NULL;
	oldpwd = NULL;
	size = ft_commandsize(mini->commandline);
	if (cd_init(command, size, &oldpwd, mini))
		return (1);
	pwd = getcwd(0, 0);
	if (!pwd && oldpwd)
		handle_dir_error(command->args[1], oldpwd, mini);
	else if (pwd && oldpwd)
		setpwds(pwd, oldpwd, mini);
	else if (!pwd && !oldpwd)
		return (go_to_root(size), 0);
	if (pwd)
		free(pwd);
	if (oldpwd)
		free(oldpwd);
	if (size > 1)
		safe_exit(0);
	return (setexit(0, 0), 0);
}
