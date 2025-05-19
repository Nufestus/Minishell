/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:56:20 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/18 17:46:07 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void heredochandle(int sig)
{
    (void)sig;
	write(STDOUT_FILENO, "\n", 1);
    exit(0);
}

void signalhandle(int sig)
{
    (void)sig;
	write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
	rl_redisplay();
}
void quithandle(int sig)
{
    (void)sig;
    printf("quit\n");
}

void callallsignals()
{
    signal(SIGINT, signalhandle);
    signal(SIGQUIT, SIG_IGN);
}
