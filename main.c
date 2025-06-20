/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 21:05:34 by rammisse          #+#    #+#             */
/*   Updated: 2025/06/03 12:26:53 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_for_essentials(char *str, int *path, int *pwd, int *shlvl)
{
	if (!ft_strcmp(str, "PWD"))
		*pwd = 1;
	if (!ft_strcmp(str, "SHLVL"))
		*shlvl = 1;
	if (!ft_strcmp(str, "PATH"))
		*path = 1;
}

void	handlesuchfile(char *str, t_commandline *command)
{
	write(2, str, ft_strlen(str));
	write(2, ": No such file or directory\n", 29);
	freedoubleint(command->mini);
	freelistcommandline(command->mini->commandline);
	clear_history();
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
	if (!isatty(0) || !isatty(1) || !isatty(2))
		exit(1);
	setupenv(env, &mini);
	if (my_getenv(&mini, "PWD"))
		mini.pwd = ft_strdup(my_getenv(&mini, "PWD"));
	readinput(&mini);
	freelistenv(mini.env);
	rl_clear_history();
}
