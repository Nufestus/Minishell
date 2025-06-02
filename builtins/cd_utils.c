/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:46:51 by rammisse          #+#    #+#             */
/*   Updated: 2025/06/01 20:45:45 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_setenv(char *envname, char *newvalue, t_minishell *mini)
{
	t_env	*env;
	char	*fullstring;
	char	*copy;

	env = mini->env;
	while (env)
	{
		if (!ft_strcmp(env->variable, envname))
		{
			if (env->value)
				free(env->value);
			env->value = ft_strdup(newvalue);
			fullstring = ft_strjoin(env->variable, "=");
			copy = fullstring;
			fullstring = ft_strjoin(fullstring, env->value);
			free(copy);
			free(env->string);
			env->string = ft_strdup(fullstring);
			free(fullstring);
			return ;
		}
		env = env->next;
	}
}

void	add_to_env(char *str, char *s, t_minishell *mini)
{
	char	*copy;
	char	*full;
	char	*string;
	t_env	*new;

	string = ft_getenv(str, mini);
	if (!string)
	{
		full = ft_strjoin(str, "=");
		copy = full;
		full = ft_strjoin(full, s);
		free(copy);
		new = ft_envnew(s, str, full);
		ft_envadd_back(&mini->env, new);
		free(full);
	}
	else
		ft_setenv(str, s, mini);
}

void	handle_dir_error(char *arg, char *oldpwd, t_minishell *mini)
{
	char	*copy;
	char	*newpwd;

	newpwd = NULL;
	perror("cd: error retrieving current directory: getcwd: \
	cannot access parent directories");
	newpwd = ft_strjoin(oldpwd, "/");
	copy = newpwd;
	newpwd = ft_strjoin(newpwd, arg);
	free(copy);
	add_to_env("PWD", newpwd, mini);
	add_to_env("OLDPWD", oldpwd, mini);
	free(newpwd);
}

void	setpwds(char *pwd, char *oldpwd, t_minishell *mini)
{
	add_to_env("PWD", pwd, mini);
	add_to_env("OLDPWD", oldpwd, mini);
}
