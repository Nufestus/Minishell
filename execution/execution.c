/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 00:13:35 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/24 20:27:43 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	file_helper(int flag, char *str, t_commandline **command, int fd)
{
	perror(str);
	if (flag == 1)
		(*command)->outfd = fd;
	else
		(*command)->infd = fd;
}

static int	skip_command(t_commandline **copy)
{
	if ((*copy)->infd == -4 || (*copy)->infd == -1)
	{
		*copy = (*copy)->next;
		return (1);
	}
	return (0);
}

static int	is_single_builtin(t_commandline *copy, int size)
{
	return (size == 1 && checkcommand(copy->cmd));
}

static void	single_builtin(t_commandline **copy, t_minishell *mini, int size)
{
	handleiosingle(copy);
	closeallpipes(mini, size);
	(*copy)->env = constructenv(mini->env);
	handlebuiltins(copy);
}

void	startpipex(t_minishell *mini)
{
	int				i;
	int				size;
	t_commandline	*copy;

	copy = mini->commandline;
	initializepipes(mini);
	size = ft_commandsize(copy);
	i = -1;
	while (++i < size && copy)
	{
		if (is_single_builtin(copy, size))
		{
			if (skip_command(&copy))
				continue ;
			single_builtin(&copy, mini, size);
			return ;
		}
		if (skip_command(&copy))
			continue ;
		childlabor(&copy);
		copy = copy->next;
	}
	setupchilds(mini, size);
}
