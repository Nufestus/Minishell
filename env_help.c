/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_help.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:00:00 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/03 14:02:34 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_env_node(char *str, t_env_setup *setup, t_minishell *mini)
{
	parse_env_string(str, setup);
	add_env_node(mini, setup, str);
	free(str);
}

void	add_essentials(t_minishell *mini, t_env_setup *setup)
{
	char	*string;
	char	*copy;

	if (!setup->pwdcheck)
	{
		string = getcwd(NULL, 0);
		if (!string)
			return ;
		copy = string;
		string = ft_strjoin("PWD=", string);
		free(copy);
		handle_env_node(string, setup, mini);
	}
	if (!setup->pathcheck)
	{
		string = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:\
/usr/sbin:/usr/bin:/sbin:/bin");
		handle_env_node(string, setup, mini);
	}
	if (!setup->shlvlcheck)
	{
		string = ft_strdup("SHLVL=1");
		handle_env_node(string, setup, mini);
	}
}
