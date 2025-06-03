/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 22:57:56 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 14:38:51 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	freelistenv(t_env *list)
{
	t_env	*copy;

	while (list)
	{
		copy = list->next;
		if (list->string)
			free(list->string);
		if (list->value)
			free(list->value);
		if (list->variable)
			free(list->variable);
		free(list);
		list = copy;
	}
}

int	ft_commandsize(t_commandline *lst)
{
	int				i;
	t_commandline	*command;

	if (!lst)
		return (0);
	i = 0;
	command = lst;
	while (command != NULL)
	{
		command = command->next;
		i++;
	}
	return (i);
}

int	ft_envsize(t_env *lst)
{
	int	i;

	if (!lst)
		return (0);
	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = 0;
		i++;
	}
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (src[len] != '\0')
		len++;
	if (dstsize == 0)
		return (len);
	while (src[i] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize > 0)
		dst[i] = '\0';
	return (len);
}
