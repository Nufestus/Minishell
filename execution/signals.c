/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:56:20 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/23 17:49:57 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signalhandle(int sig)
{
	const char	c = '\n';

	(void)sig;
	rl_redisplay();
	rl_on_new_line();
	rl_replace_line("\n", 0);
	ioctl(0, TIOCSTI, &c);
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
