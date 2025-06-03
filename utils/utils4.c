/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 23:00:20 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 14:40:41 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	freelisttokens(t_tokenize *list)
{
	t_tokenize	*copy;

	while (list)
	{
		copy = list->next;
		free(list->str);
		free(list);
		list = copy;
	}
}

static int	handlereturn(unsigned char c, unsigned char b)
{
	if (c - b > 0)
		return (1);
	else if (c - b < 0)
		return (-1);
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((str1[i] != '\0' || str2[i] != '\0') && i < n)
	{
		if (str1[i] != str2[i])
			return (handlereturn(str1[i], str2[i]));
		i++;
	}
	return (0);
}

t_commandline	*ft_commandlast(t_commandline *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	initializetonone(char **str, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		str[i] = NULL;
		i++;
	}
}
