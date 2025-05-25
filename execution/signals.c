/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:56:20 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 01:24:52 by rammisse         ###   ########.fr       */
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
	g_sig = 130;
}

void	quithandle(int sig)
{
	(void)sig;
	printf("quit\n");
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
	g_sig = 130;
}

int	setexit(int exitstatus, int flag)
{
	static int	res;

	if (flag == 0)
		res = exitstatus;
	return (res);
}
