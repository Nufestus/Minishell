/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:23 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/03 19:54:38 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_default_env(void)
{
	char	**newenv;
	char	*str;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	newenv = malloc(sizeof(char *) * 4);
	if (!newenv)
		return (free(cwd), exit(1), NULL);
	str = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	newenv[0] = ft_strjoin("PWD=", cwd);
	newenv[1] = ft_strdup("SHLVL=1");
	newenv[2] = ft_strdup(str);
	newenv[3] = NULL;
	free(cwd);
	return (newenv);
}

void	parse_env_string(char *env_str, t_env_setup *setup)
{
	setup->len = 0;
	while (env_str[setup->len] && env_str[setup->len] != '=')
		setup->len++;
	setup->var = ft_substr(env_str, 0, setup->len);
	setup->start = setup->len;
	while (env_str[setup->len])
		setup->len++;
	setup->value = ft_substr(env_str, setup->start + 1, setup->len);
	if (setup->value[0] == '\0')
	{
		free(setup->value);
		setup->value = NULL;
	}
	setup->string = ft_strdup(env_str);
}

void	add_env_node(t_minishell *mini, t_env_setup *setup, char *path_ref)
{
	t_env	*node;

	node = ft_envnew(setup->value, setup->var, setup->string);
	if (node && (!setup->value || !ft_strcmp(node->string, path_ref)))
		node->isexported = 1;
	ft_envadd_back(&mini->env, node);
	free(setup->var);
	free(setup->value);
	free(setup->string);
}

void	process_env_array(char **env_array, t_minishell *mini)
{
	int			i;
	t_env_setup	setup;

	i = 0;
	setup.pathcheck = 0;
	setup.pwdcheck = 0;
	setup.shlvlcheck = 0;
	while (env_array[i])
	{
		parse_env_string(env_array[i], &setup);
		if (setup.value)
		{
			add_env_node(mini, &setup, env_array[i]);
			check_for_essentials(ft_envlast(mini->env)->variable,
				&setup.pathcheck, &setup.pwdcheck, &setup.shlvlcheck);
		}
		else
		{
			free(setup.var);
			free(setup.string);
		}
		i++;
	}
	add_essentials(mini, &setup);
}

void	setupenv(char **env, t_minishell *mini)
{
	char	**env_array;

	if (!*env)
	{
		env_array = create_default_env();
		if (!env_array)
			return ;
		process_env_array(env_array, mini);
		freedoublearray(env_array);
	}
	else
		process_env_array(env, mini);
}
