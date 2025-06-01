/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:22:50 by aammisse          #+#    #+#             */
/*   Updated: 2025/06/01 20:00:55 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expandhelp1(t_expanding *expand, char *str)
{
	expand->i++;
	if (str[expand->i] == '?')
		expand->expandedvar = ft_itoa(setexit(0, 1));
	expand->i++;
	expand->k = 0;
	while (expand->expandedvar[expand->k])
	{
		expand->expanded[expand->j] = expand->expandedvar[expand->k];
		expand->j++;
		expand->k++;
	}
	free(expand->expandedvar);
}

int	expandhelp2(t_expanding *expand, char *str, int *check)
{
	if (str[expand->i] == '\'' && !expand->indouble)
	{
		expand->insingle = !expand->insingle;
		expand->i++;
		return (1);
	}
	else if (str[expand->i] == '"' && !expand->insingle)
	{
		expand->indouble = !expand->indouble;
		expand->i++;
		return (1);
	}
	if (expand->indouble || expand->insingle)
		if (check)
			*check = 2;
	return (0);
}

int	expandhelp3(t_expanding *expand, char *str, t_minishell *mini, int *check)
{
	if (ft_isalpha(str[expand->i + 1]) || str[expand->i + 1] == '_')
	{
		if (!expandhelp(expand, str, check, mini))
			return (0);
	}
	else if (str[expand->i + 1] == '?')
		expandhelp1(expand, str);
	else if (str[expand->i] == '$' && str[expand->i + 1] == '$')
		expand->i += 2;
	else if (str[expand->i] == '$' && ft_isdigit(str[expand->i + 1]))
		expand->i += 2;
	else
		expand->expanded[expand->j++] = str[expand->i++];
	return (1);
}

char	*expand(int *check, char *str, t_minishell *mini)
{
	t_expanding	expand;

	initilizeexpand(mini, str, &expand);
	if (!expand.expanded)
		return (NULL);
	while (str[expand.i])
	{
		if (expandhelp2(&expand, str, check) == 1)
			continue ;
		if (str[expand.i] == '$' && !expand.insingle && str[expand.i + 1])
		{
			mini->expanded = 1;
			if (!expandhelp3(&expand, str, mini, check))
				return (NULL);
		}
		else
			expand.expanded[expand.j++] = str[expand.i++];
	}
	expand.expanded[expand.j] = '\0';
	return (expand.expanded);
}

char	*expandinheredoc(int *check, char *str, t_minishell *mini)
{
	t_expanding	expand;

	initilizeexpand(mini, str, &expand);
	if (!expand.expanded)
		return (NULL);
	while (str[expand.i])
	{
		if (str[expand.i] == '$' && str[expand.i + 1])
		{
			mini->expanded = 1;
			if (!expandhelp3(&expand, str, mini, check))
				return (NULL);
		}
		else
			expand.expanded[expand.j++] = str[expand.i++];
	}
	expand.expanded[expand.j] = '\0';
	return (expand.expanded);
}
