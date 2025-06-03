/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:31:14 by rammisse          #+#    #+#             */
/*   Updated: 2025/06/03 12:46:10 by aammisse         ###   ########.fr       */
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

void	exporthelp(t_commandline *command, t_builtin *export)
{
	char	**array;

	export->new = command->mini->env;
	array = NULL;
	array = tosortarray(command);
	array = sortarray(array);
	if (export->size > 1)
	{
		printarray(array);
		setexit(0, 0);
		freedoublearray(array);
		safe_exit(0);
	}
	else
	{
		printarray(array);
		setexit(0, 0);
		freedoublearray(array);
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
