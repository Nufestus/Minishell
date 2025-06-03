/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:06:43 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/01 18:12:26 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_sig = 0;

int	getinput(int delflag, char *del, t_minishell *mini)
{
	int		fd[2];
	char	*line;
	char	*copy;

	if (pipe(fd))
		return (perror("pipe"), exit(1), 0);
	signal(SIGINT, heredochandle);
	while (1)
	{
		line = readline("> ");
		if (g_sig == 130)
		{
			getinputhelp2(fd, &mini, line);
			g_sig = 0;
			return (-4);
		}
		if (getinputhelp3(line, mini, del))
			break ;
		copy = line;
		if (getinputhelp4(line, del))
			break ;
		getinputhelp1(delflag, &line, copy, mini);
		getinputhelp(line, fd[1]);
	}
	return (callallsignals(), close(fd[1]), fd[0]);
}

int	readinputhelp(t_minishell **mini)
{
	if (!(*mini)->input)
	{
		free((*mini)->input);
		printf("%s\n", "exit");
		safe_exit(1);
	}
	if ((*mini)->input[0] == '\0')
	{
		free((*mini)->input);
		return (1);
	}
	add_history((*mini)->input);
	if (tokenize(*mini) == -1)
	{
		freelisttokens((*mini)->tokens);
		(*mini)->tokens = NULL;
		free((*mini)->input);
		(*mini)->exitstatus = 2;
		return (1);
	}
	return (0);
}

int	readinputhelp1(t_minishell **mini)
{
	if ((*mini)->check != 2)
		parse(*mini);
	else
	{
		free((*mini)->input);
		freelisttokens((*mini)->tokens);
		(*mini)->tokens = NULL;
		return (closeallfiles(*mini), closeallheredocs(*mini), exit(1), 0);
	}
	reparse(*mini);
	if ((*mini)->check == 1)
	{
		(*mini)->check = 0;
		(*mini)->exitstatus = 2;
		freelisttokens((*mini)->tokens);
		return ((*mini)->tokens = NULL, free((*mini)->input), 1);
	}
	if (setupcommandline(*mini))
	{
		freelistcommandline((*mini)->commandline);
		freelisttokens((*mini)->tokens);
		return ((*mini)->tokens = NULL, free((*mini)->input), 1);
	}
	freelisttokens((*mini)->tokens);
	return ((*mini)->tokens = NULL, 0);
}

void	readinput(t_minishell *mini)
{
	while (1)
	{
		callallsignals();
		mini->input = readline(INPUT1);
		if (g_sig == 130)
		{
			setexit(130, 0);
			g_sig = 0;
		}
		mini->linecount++;
		if (readinputhelp(&mini))
			continue ;
		checkheredocs(mini);
		if (readinputhelp1(&mini))
			continue ;
		openallfiles(mini);
		execute(mini);
		closeallheredocs(mini);
		freedoubleint(mini);
		freelistcommandline(mini->commandline);
	}
}
