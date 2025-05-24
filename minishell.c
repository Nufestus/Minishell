/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:23 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/23 17:09:22 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal = 0;

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
	if (!*env)
	{
		int z = 0;
		char *cwd = getcwd(NULL, 0);
		char *path = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
		char *shlvl = ft_strdup("SHLVL=1");
		char *pwd = ft_strjoin("PWD=", cwd);
		char **newenv = malloc(sizeof(char *) * (4));
		newenv[0] = pwd;
		newenv[1] = shlvl;
		newenv[2] = path;
		newenv[3] = NULL;
		free(cwd);
		while(newenv[z])
		{
			int l = 0;
			while(newenv[z][l] && newenv[z][l] != '=')
				l++;
			var = ft_substr(newenv[z], 0, l);
			start = l;
			while(newenv[z][l])
				l++;
			value = ft_substr(newenv[z], start + 1, l);
			string = ft_strdup(newenv[z]);
			node = ft_envnew(value, var, string);
			if (!value)
				node->isexported = 1;
			if (!ft_strcmp(node->string, path))
				node->isexported = 1;
			ft_envadd_back(&mini->env, node);
			free(var);
			free(value);
			free(string);
			z++;
		}
		freedoublearray(newenv);
	}
	else
	{
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
			if (!value)
				node->isexported = 1;
			ft_envadd_back(&mini->env, node);
			free(var);
			free(value);
			free(string);
			i++;
		}
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
	mini.linecount = 0;
	callallsignals();
	if (!isatty(0))
		return (1);
	setupenv(env, &mini);
	readinput(&mini);
	freelistenv(mini.env);
	rl_clear_history();
}