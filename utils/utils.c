/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:48:34 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/29 12:13:12 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(char *s1)
{
	size_t	i;
	char	*copy;
	char	*str;

	i = 0;
	if (!s1)
		return (NULL);
	str = (char *)s1;
	copy = (char *) malloc(strlen(str) + 1);
	if (copy == NULL)
		exit(1);
	while (s1[i] != '\0')
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

void	freedoublearray(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	freelistcommandline(t_commandline *list)
{
	t_commandline	*copy;

	if (!list)
		return ;
	while (list)
	{
		copy = list->next;
		if (list->args)
			freedoublearray(list->args);
		if (list->env)
			freedoublearray(list->env);
		freelistfiles(list->file);
		if (list->cmd)
			free(list->cmd);
		free(list);
		list = copy;
	}
}

void	freelistfiles(t_files *list)
{
	t_files	*copy;

	while (list)
	{
		copy = list->next;
		if (list->file)
			free(list->file);
		if (list->delimiter)
			free(list->delimiter);
		free(list);
		list = copy;
	}
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == c)
		return ((char *)s);
	else
		return (NULL);
}
