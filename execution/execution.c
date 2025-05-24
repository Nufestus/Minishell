/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 00:13:35 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/23 22:58:10 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	startpipex(t_minishell *mini)
{
	int				i;
	int				size;
	t_commandline	*copy;

	i = -1;
	copy = mini->commandline;
	initializepipes(mini);
	size = ft_commandsize(copy);
	while (++i < size)
	{
		if (size > 1 || !checkcommand(copy->cmd))
		{
			if (copy->infd == -4)
			{
				copy = copy->next;
				continue;
			}
			childlabor(&copy);
		}
		else if (size == 1)
		{
			if (copy->infd == -1)
			{
				copy = copy->next;
				continue;
			}
			handleiosingle(&copy);
			closeallpipes(mini, size);
			copy->env = constructenv(mini->env);
			handlebuiltins(&copy);
			return ;
		}
		copy = copy->next;
	}
	setupchilds(mini, size);
}

void	execute(t_minishell *mini)
{
	signal(SIGQUIT, signalhandle);
	startpipex(mini);
}
