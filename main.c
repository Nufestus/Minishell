/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 21:05:34 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/31 16:15:58 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handlesuchfile(char *str, t_commandline *command)
{
	write(2, str, ft_strlen(str));
	write(2, ": No such file or directory\n", 29);
	freedoubleint(command->mini);
	freelistcommandline(command->mini->commandline);
	exit(127);
}

void	initializemini(t_minishell *mini)
{
	mini->env = NULL;
	mini->commandline = NULL;
	mini->input = NULL;
	mini->tokens = NULL;
	mini->envstate = 0;
	mini->exitstatus = 0;
	mini->check = 0;
	mini->linecount = 0;
	mini->pwd = NULL;
	mini->flag = 0;
}

int	main(int ac, char **av, char **env)
{
	t_minishell	mini;

	if (ac > 1)
	{
		printf("%s: no such file or directory\n", av[1]);
		exit(127);
	}
	initializemini(&mini);
	callallsignals();
	if (!isatty(0))
		exit(1);
	setupenv(env, &mini);
	readinput(&mini);
	freelistenv(mini.env);
	rl_clear_history();
}
