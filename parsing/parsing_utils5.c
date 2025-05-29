/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:56:50 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/29 18:13:32 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	in_set(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	removequoteshelp(int *flag, t_parse *parse, char *str)
{
	if (str[parse->i] == '\'' && !parse->indouble)
	{
		parse->insingle = !parse->insingle;
		parse->i++;
		if (flag && *flag == 0)
			*flag = 3;
		return (1);
	}
	else if (str[parse->i] == '"' && !parse->insingle)
	{
		parse->indouble = !parse->indouble;
		parse->i++;
		if (flag && *flag == 0)
			*flag = 3;
		return (1);
	}
	else
		parse->expanded[parse->j++] = str[parse->i++];
	return (0);
}

char	*removequotes(int *flag, char *str)
{
	t_parse	parse;

	parse.total = ft_strlen(str);
	parse.insingle = 0;
	parse.indouble = 0;
	parse.i = 0;
	parse.j = 0;
	parse.expanded = malloc(parse.total + 1);
	if (!parse.expanded)
		return (NULL);
	while (str[parse.i])
	{
		if (removequoteshelp(flag, &parse, str))
			continue ;
	}
	parse.expanded[parse.j] = '\0';
	return (parse.expanded);
}

void	initreparse(t_reparse *reparse, char *str)
{
	reparse->i = 0;
	reparse->j = 0;
	reparse->exportcheck = 0;
	reparse->copy = str;
	reparse->prev = NULL;
}
