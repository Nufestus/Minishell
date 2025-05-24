/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:35:36 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/21 16:18:39 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	freeunset(t_env *curr)
{
	free(curr->string);
	free(curr->value);
	free(curr->variable);
	free(curr);
}

void	lstremoveif(t_env **env, char *value)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (!ft_strcmp(curr->variable, value))
		{
			if (prev == NULL)
				*env = curr->next;
			else
				prev->next = curr->next;
			freeunset(curr);
			if (prev == NULL)
				curr = *env;
			else
				curr = prev->next;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

void	unset(t_commandline *commandline)
{
	int		size;
	int		i;
	char	**str;

	i = 1;
	size = ft_commandsize(commandline->mini->commandline);
	str = commandline->args;
	commandline->mini->exitstatus = 0;
	while (str[i])
	{
		lstremoveif(&commandline->mini->env, str[i]);
		i++;
	}
	if (size > 1)
		exit(0);
	commandline->mini->exitstatus = 1;
}
