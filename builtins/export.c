/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:03:09 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/31 16:11:20 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*getstring(char *str)
{
	int		i;
	int		j;
	int		skip;
	char	*string;

	i = 0;
	j = 0;
	skip = 0;
	while (str[i])
		i++;
	string = malloc(i + 1);
	if (!string)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (!skip && str[i] == '+' && str[i + 1] == '=')
		{
			skip = 1;
			i++;
		}
		string[j++] = str[i++];
	}
	return (string[j] = '\0', string);
}

void	exporthelp4(t_commandline *command, t_builtin *export)
{
	export->new = getenvnode(command->mini->env, export->var);
	export->tmp = export->new->value;
	export->new->value = ft_strdup(export->value);
	free(export->tmp);
	export->tmp = export->new->string;
	export->new->string = getstring(export->str[export->i]);
	free(export->tmp);
	setexit(0, 0);
}

void	exportstatements(t_commandline *command, t_builtin *export)
{
	if (checkvalid(export->str[export->i], command->mini) == 1)
		exporthelp2(command, export);
	else if (checkvalid(export->str[export->i], command->mini) == 2)
		exporthelp3(command, export);
	else if (checkvalid(export->str[export->i], command->mini) == 3)
		exporthelp4(command, export);
	else if (checkvalid(export->str[export->i], command->mini) == 0)
	{
		printf("export: not a valid identifier: %s\n", export->str[export->i]);
		setexit(1, 0);
	}
	else if (checkvalid(export->str[export->i], command->mini) == 4)
		exporthelp5(command, export);
}

void	exporthelp5(t_commandline *command, t_builtin *export)
{
	export->new = ft_envnew(export->value, export->var, export->string);
	free(export->new->value);
	export->new->value = NULL;
	ft_envadd_back(&command->mini->env, export->new);
	setexit(0, 0);
}

void	export(t_commandline *command)
{
	t_builtin	export;

	export.i = 1;
	export.size = ft_commandsize(command->mini->commandline);
	export.str = command->args;
	if (!export.str[1] || !export.str[1][0])
	{
		exporthelp(command, &export);
		return ;
	}
	while (export.str[export.i])
	{
		export.value = getvalue(export.str[export.i]);
		export.var = getvar(export.str[export.i]);
		export.string = getstring(export.str[export.i]);
		exportstatements(command, &export);
		free(export.value);
		free(export.string);
		free(export.var);
		export.i++;
	}
	if (export.size > 1)
		exit(0);
}
