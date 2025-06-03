/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:42:23 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/29 11:54:32 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	help_openfile(t_commandline *command, t_files *file)
{
	if (file->type == REDIN || file->type == HEDOC)
	{
		if (command->infd > 2)
			close(command->infd);
		command->infd = dup(file->fd);
	}
	else if (file->type == REDOUT || file->type == APPEND)
	{
		if (command->outfd > 2)
			close(command->outfd);
		command->outfd = dup(file->fd);
	}
}

int	openfiles(t_commandline **command, t_minishell *mini)
{
	t_files		*files;

	(void)mini;
	files = (*command)->file;
	while (files)
	{
		if (files->type == REDIN)
			files->fd = open(files->file, O_RDONLY);
		else if (files->type == HEDOC)
			files->fd = files->hedoc;
		if (files->type == REDOUT)
			files->fd = open(files->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		else if (files->type == APPEND)
			files->fd = open(files->file, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (files->fd == -1)
		{
			perror(files->file);
			help_openfile(*command, files);
			return (setexit(1, 0), 1);
		}
		help_openfile(*command, files);
		close(files->fd);
		files = files->next;
	}
	return (0);
}

void	freestr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*my_getenv(t_minishell *mini, char *str)
{
	t_env	*env;

	env = mini->env;
	while (env)
	{
		if (!ft_strcmp(env->variable, str))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	ft_find(char *str, char *del)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (del[j])
		{
			if (str[i] == del[j])
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}
