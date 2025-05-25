/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:18:37 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 14:29:30 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strtrim(char *s1, char *set)
{
	size_t	startind;
	size_t	lastind;
	char	*trim;

	if (s1 != NULL && set == NULL)
		return (alloc((char *)s1));
	else if ((s1 == NULL && set == NULL) || (s1 == NULL && set != NULL))
		return (NULL);
	startind = 0;
	lastind = ft_strlen((char *)s1) - 1;
	while (in_set(s1[startind], (char *)set) && s1[startind] != '\0')
		startind++;
	if (s1[startind] == '\0')
		return (ft_strdup(""));
	while (in_set(s1[lastind], (char *)set) && lastind > startind)
		lastind--;
	trim = (char *)malloc(lastind - startind + 2);
	if (trim == NULL)
		return (NULL);
	ft_strlcpy(trim, s1 + startind, lastind - startind + 2);
	return (trim);
}

int	countdouble(char *str, char *delims)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (!is_delim(str[i], delims) && (is_delim(str[i + 1], delims)
				|| str[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

void	countwordhelp(int *i, char *s, char q, int *k)
{
	while (s[*i] && !ft_strchr(" \t\n\r\v\f", s[*i]))
	{
		if (s[*i] == '"' || s[*i] == '\'')
		{
			q = s[*i];
			(*i)++;
			(*k)++;
			while (s[*i] && s[*i] != q)
			{
				(*k)++;
				(*i)++;
			}
			if (s[*i] == q)
			{
				(*i)++;
				(*k)++;
			}
		}
		else
		{
			(*k)++;
			(*i)++;
		}
	}
}

int	countword(char **str)
{
	int		i;
	int		k;
	int		copy;
	char	q;
	char	*s;

	i = 0;
	k = 0;
	s = *str;
	q = 0;
	while (s[i])
	{
		while (s[i] && ft_strchr(" \t\n\r\v\f", s[i]))
			i++;
		if (!s[i])
			break ;
		k = 0;
		countwordhelp(&i, s, q, &k);
		copy = i;
		while (copy-- > 0)
			(*str)++;
		return (k);
	}
	return (k);
}

void	handlenodes(int split, char *str, int i, t_tokenize **tokens)
{
	t_setupnode	setup;

	setup.split = split;
	setup.index = i;
	setup.str = str;
	if (!split || split == 2 || split == 3)
	{
		if (!ft_strcmp(str, "|"))
			setupnode(&setup, 0, PIPE, tokens);
		else if (!ft_strcmp(str, ">>"))
			setupnode(&setup, 1, APPEND, tokens);
		else if (!ft_strcmp(str, "<<"))
			setupnode(&setup, 1, HEDOC, tokens);
		else if (!ft_strcmp(str, ">"))
			setupnode(&setup, 1, REDOUT, tokens);
		else if (!ft_strcmp(str, "<"))
			setupnode(&setup, 1, REDIN, tokens);
		else if (isanoption(str))
			setupnode(&setup, 0, OPTION, tokens);
		else
			setupnode(&setup, 0, WORD, tokens);
	}
	else
		setupnode(&setup, 0, WORD, tokens);
}
