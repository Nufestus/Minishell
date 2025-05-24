/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput_utils5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 01:09:56 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/24 01:30:58 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	getinputhelp(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 2);
	free(line);
}

void	getinputhelp1(int delflag, char **line, char *copy, t_minishell *mini)
{
	if (!delflag)
	{
		*line = expand(NULL, *line, mini);
		free(copy);
	}
}

void	getinputhelp2(int *fd, t_minishell **mini, char *line)
{
	close(fd[0]);
	close(fd[1]);
	callallsignals();
	(*mini)->exitstatus = 130;
	free(line);
}

int	getinputhelp3(char *line, t_minishell *mini, char *del)
{
	if (!line)
	{
		if (mini)
		{
			printf("warning: here-document at line %d ", mini->linecount);
			printf("delimited by end-of-file (wanted '%s')\n", del);
		}
		free(line);
		return (1);
	}
	return (0);
}

int	getinputhelp4(char *line, char *del)
{
	if (!ft_strcmp(line, del))
	{
		free(line);
		return (1);
	}
	return (0);
}
