/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:56:20 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/29 18:24:35 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signalhandle(int sig)
{
	(void)sig;
	write(1, "\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	setexit(130, 0);
}

void	callallsignals(void)
{
	signal(SIGINT, signalhandle);
	signal(SIGQUIT, SIG_IGN);
}

void	normalhande(int sig)
{
	(void)sig;
	write(1, "\n", 2);
	g_sig = 140;
	setexit(130, 0);
}

int	setexit(int exitstatus, int flag)
{
	static int	res;

	if (flag == 0)
		res = exitstatus;
	return (res);
}

void	initializechild(t_setupchild *child, int size, t_minishell *mini)
{
	child->i = -1;
	child->skip = 0;
	child->copy = mini->commandline;
	closeallpipes(mini, size);
}
