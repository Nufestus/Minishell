/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:24:27 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 14:33:19 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	commandlinehelp(t_commandline_setup *setup)
{
	while (setup->token && setup->token->type != PIPE)
	{
		if (setup->token->type == CMD)
			setup->cmd = setup->token->str;
		else if (setup->token->type == ARG)
			setup->arg[setup->argcount++] = ft_strdup(setup->token->str);
		else if (setup->token->type == OPTION)
			setup->arg[setup->argcount++] = ft_strdup(setup->token->str);
		setup->token = setup->token->next;
	}
}

void	commandlinehelp1(t_commandline_setup *setup, t_minishell *mini)
{
	setup->arg[setup->argcount] = NULL;
	setup->command = ft_commandnew(setup->cmd, setup->arg);
	setup->command->index = setup->index;
	setup->command->mini = mini;
	setup->command->argcount = setup->argcount;
	ft_commandadd_back(&mini->commandline, setup->command);
	freedoublearray(setup->arg);
	setup->index++;
}

void	initvarscmd(t_commandline_setup *setup, t_minishell *mini)
{
	setup->token = mini->tokens;
	setup->file = mini->tokens;
	setup->command = NULL;
	setup->copy = NULL;
	mini->commandline = NULL;
	setup->index = 0;
}

void	settozero(t_commandline_setup *setup)
{
	setup->count = 0;
	setup->cmd = NULL;
	setup->arg = NULL;
	setup->option = NULL;
	setup->argcount = 0;
}

void	setupcommandline(t_minishell *mini)
{
	t_commandline_setup	setup;

	initvarscmd(&setup, mini);
	while (setup.token)
	{
		settozero(&setup);
		if (setup.token->index == 0)
		{
			setup.count = getarguments(setup.token);
			setup.arg = malloc(sizeof(char *) * (setup.count + 3));
			commandlinehelp(&setup);
			commandlinehelp1(&setup, mini);
		}
		else if (setup.token->type == PIPE)
		{
			setup.token = setup.token->next;
			if (!setup.token)
				break ;
			setup.count = getarguments(setup.token);
			setup.arg = malloc(sizeof(char *) * (setup.count + 3));
			commandlinehelp(&setup);
			commandlinehelp1(&setup, mini);
		}
	}
	addfile(setup.file, mini->commandline);
}
