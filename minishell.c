/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:23 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/29 09:48:56 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void setupenv(char **env, t_minishell *mini)
{
	char *var;
	char *value;
	char *string;
	t_env *node;
	int i;
	int j;
	int start;

	i = 0;
	j = 0;
	while(env[i])
	{
		j = 0;
		while(env[i][j] && env[i][j] != '=')
			j++;
		var = ft_substr(env[i], 0, j);
		start = j;
		while(env[i][j])
			j++;
		value = ft_substr(env[i], start + 1, j);
		string = ft_strdup(env[i]);
		node = ft_envnew(value, var, string);
		ft_envadd_back(&mini->env, node);
		free(var);
		free(value);
		free(string);
		i++;
	}
}

int	main(int ac, char **av, char **env)
{
	t_minishell mini;

	(void)ac;
	(void)av;
	mini.env = NULL;
	mini.commandline = NULL;
	mini.input = NULL;
	mini.tokens = NULL;
	mini.exitstatus = 0;
	mini.check = 0;
	setupenv(env, &mini);
	readinput(&mini);
	freelistenv(mini.env);
}