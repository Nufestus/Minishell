/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:23 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/25 14:43:31 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_default_env(void)
{
	char	**newenv;
	char	*str;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	newenv = malloc(sizeof(char *) * 4);
	str = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	if (!newenv)
		return (NULL);
	newenv[0] = ft_strjoin("PWD=", cwd);
	newenv[1] = ft_strdup("SHLVL=1");
	newenv[2] = ft_strdup(str);
	newenv[3] = NULL;
	free(cwd);
	return (newenv);
}

static void	parse_env_string(char *env_str, t_env_setup *setup)
{
	setup->len = 0;
	while (env_str[setup->len] && env_str[setup->len] != '=')
		setup->len++;
	setup->var = ft_substr(env_str, 0, setup->len);
	setup->start = setup->len;
	while (env_str[setup->len])
		setup->len++;
	setup->value = ft_substr(env_str, setup->start + 1, setup->len);
	setup->string = ft_strdup(env_str);
}

static void	add_env_node(t_minishell *mini, t_env_setup *setup, char *path_ref)
{
	t_env	*node;

	node = ft_envnew(setup->value, setup->var, setup->string);
	if (!setup->value || !ft_strcmp(node->string, path_ref))
		node->isexported = 1;
	ft_envadd_back(&mini->env, node);
	free(setup->var);
	free(setup->value);
	free(setup->string);
}

static void	process_env_array(char **env_array, t_minishell *mini)
{
	t_env_setup	setup;
	int			i;

	i = 0;
	while (env_array[i])
	{
		parse_env_string(env_array[i], &setup);
		add_env_node(mini, &setup, env_array[2]);
		i++;
	}
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
