/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:50:38 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/27 16:25:13 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	quotedhelper(char quote, char *str, int *i, int *len)
{
	if (str[*i] == '\'' || str[*i] == '"')
	{
		quote = str[*i];
		(*i)++;
		while (str[*i] && str[*i] != quote)
		{
			(*len)++;
			(*i)++;
		}
		if (str[*i] == quote)
			(*i)++;
	}
	else
	{
		(*len)++;
		(*i)++;
	}
}

int	quoted(char *str)
{
	int		i;
	int		len;
	char	quote;

	i = 0;
	len = 0;
	quote = 0;
	while (str[i])
		quotedhelper(quote, str, &i, &len);
	return (len);
}

void	initilizeexpand(t_minishell *mini, char *str, t_expanding *expand)
{
	expand->total = varlen(str, mini) + ft_strlen(str);
	expand->insingle = 0;
	expand->indouble = 0;
	expand->i = 0;
	expand->j = 0;
	expand->start = 0;
	expand->len = 0;
	expand->k = 0;
	expand->expanded = malloc(expand->total + 1);
}

int	expandhelp(t_expanding *expand, char *str, int *check, t_minishell *mini)
{
	expand->i++;
	expand->start = expand->i;
	while (str[expand->i] && ft_isalnumm(str[expand->i]))
		expand->i++;
	expand->len = expand->i - expand->start;
	expand->var = ft_substr(str, expand->start, expand->len);
	if (!expand->var)
		return (free(expand->expanded), 0);
	if (!expand->indouble && check)
		*check = 1;
	expand->expandedvar = ft_getenv(expand->var, mini);
	if (expand->expandedvar)
	{
		expand->k = 0;
		while (expand->expandedvar[expand->k])
		{
			expand->expanded[expand->j] = expand->expandedvar[expand->k];
			expand->j++;
			expand->k++;
		}
	}
	return (free(expand->var), 1);
}
