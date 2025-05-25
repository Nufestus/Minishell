/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:31:14 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 18:30:49 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*getenvnode(t_env *env, char *var)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(var, tmp->variable))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	exportutil(t_builtin *export, t_commandline *command)
{
	while (export->new)
	{
		ft_putstr_fd("declare -x ", command->outfd);
		ft_putstr_fd(export->new->variable, command->outfd);
		if (export->new->value)
		{
			ft_putstr_fd("=\"", command->outfd);
			ft_putstr_fd(export->new->value, command->outfd);
			ft_putstr_fd("\"\n", command->outfd);
		}
		else
			ft_putstr_fd("\n", command->outfd);
		export->new = export->new->next;
	}
}

void	exporthelp(t_commandline *command, t_builtin *export)
{
	export->new = command->mini->env;
	if (export->size > 1)
	{
		while (export->new)
		{
			if (!export->new->value)
				printf("declare -x %s=\"%s\"\n", export->new->variable,
					export->new->value);
			else
				printf("declare -x %s\n", export->new->variable);
			export->new = export->new->next;
		}
		setexit(0, 0);
		exit(0);
	}
	else
	{
		exportutil(export, command);
		setexit(0, 0);
	}
}

void	exporthelp2(t_commandline *command, t_builtin *export)
{
	export->new = ft_envnew(export->value, export->var, export->string);
	ft_envadd_back(&command->mini->env, export->new);
	export->new->isexported = false;
	setexit(0, 0);
}

void	exporthelp3(t_commandline *command, t_builtin *export)
{
	export->new = getenvnode(command->mini->env, export->var);
	if (export->new)
	{
		export->tmp = export->new->value;
		export->new->value = ft_strjoin(export->new->value, export->value);
		free(export->tmp);
		export->tmp = export->new->string;
		export->new->string = ft_strjoin(export->new->string, export->value);
		free(export->tmp);
	}
	else
	{
		export->tmp = export->string;
		export->string = getstring(export->string);
		free(export->tmp);
		export->new = ft_envnew(export->value, export->var, export->string);
		export->new->isexported = false;
		ft_envadd_back(&command->mini->env, export->new);
	}
	setexit(0, 0);
}
