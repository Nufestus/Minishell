/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:43:23 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/01 18:17:33 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf))
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

void	directoryerror(char *s)
{
	write(2, s, ft_strlen(s));
	write(2, ": Is a directory\n", 18);
	free(s);
	safe_exit(126);
}

char	*checkfile(t_commandline *command)
{
	int		i;
	char	*str;
	char	*tmp;
	char	**path;

	i = 0;
	tmp = my_getenv(command->mini, "PATH");
	path = split(tmp, ":");
	if (!path)
		return (NULL);
	i = -1;
	while (path[++i])
	{
		str = ft_strjoin(path[i], "/");
		tmp = str;
		str = ft_strjoin(str, command->args[0]);
		free(tmp);
		if (!access(str, F_OK | X_OK))
			return (freestr(path), str);
		free(str);
	}
	return (freestr(path), NULL);
}

char	**constructenv(t_env *env)
{
	int		i;
	int		size;
	char	**res;
	t_env	*copy;

	i = 0;
	copy = env;
	size = ft_envsize(env);
	res = malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (exit(1), NULL);
	while (i < size)
	{
		res[i] = ft_strdup(copy->string);
		copy = copy->next;
		i++;
	}
	res[i] = NULL;
	return (res);
}

void	initializepipes(t_minishell *mini)
{
	int	size;
	int	i;

	i = 0;
	size = ft_commandsize(mini->commandline);
	mini->pipes = malloc((size) * sizeof(int *));
	while (i < size)
	{
		mini->pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(mini->pipes[i]) == -1)
		{
			perror("pipe");
			safe_exit(1);
		}
		i++;
	}
}
