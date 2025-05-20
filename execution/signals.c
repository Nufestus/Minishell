/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 09:56:20 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/20 21:37:17 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void signalhandle(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 2);
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
