/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:42:23 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/25 01:21:50 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	open_outfiles(t_commandline **command, int infd)
{
	int			outfd;
	t_files		*outfiles;

	outfd = -2;
	(*command)->infd = infd;
	outfiles = (*command)->outfile;
	while (outfiles)
	{
		if (outfiles->type == REDOUT)
			outfd = open(outfiles->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		else if (outfiles->type == APPEND)
			outfd = open(outfiles->file, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (outfiles->next && outfd != -1)
			close(outfd);
		if (outfd == -1)
		{
			perror(outfiles->file);
			(*command)->outfd = outfd;
			return ;
		}
		outfiles = outfiles->next;
	}
	(*command)->outfd = outfd;
}

int	openfiles(t_commandline **command, t_minishell *mini)
{
	int			infd;
	char		*del;
	t_files		*infiles;

	infd = -2;
	infiles = (*command)->infile;
	while (infiles)
	{
		if (infiles->type != HEDOC)
			infd = open(infiles->file, O_RDONLY);
		else if (infiles->type == HEDOC)
		{
			del = ft_strdup(infiles->delimiter);
			infd = getinput(infiles->delinquotes, del, mini);
			free(del);
			if (infd == -4)
				return (1);
		}
		if (infd == -1)
			return (perror(infiles->file), (*command)->infd = infd, 0);
		if (infiles->next && infd > 2)
			close(infd);
		infiles = infiles->next;
	}
	return (open_outfiles(command, infd), 0);
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
