/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:56:20 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/24 00:34:53 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    signalhandle(int sig)
{
	const char	c = '\n';
	(void)sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	ioctl(0, TIOCSTI, &c);
	g_sig = 130;
}
void    quithandle(int sig)
{
    (void)sig;
    printf("quit\n");
}

void    callallsignals()
{
    signal(SIGINT, signalhandle);
    signal(SIGQUIT, SIG_IGN);
}
