/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 21:05:34 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/24 21:06:37 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_minishell	mini;

	(void)ac;
	(void)av;
	mini.env = NULL;
	mini.commandline = NULL;
	mini.input = NULL;
	mini.tokens = NULL;
	mini.envstate = 0;
	mini.exitstatus = 0;
	mini.check = 0;
	mini.linecount = 0;
	callallsignals();
	setupenv(env, &mini);
	readinput(&mini);
	freelistenv(mini.env);
	rl_on_new_line();
	rl_redisplay();
}
