/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 23:01:53 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/22 13:46:59 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *str, char *str1)
{
	int	i;

	i = 0;
	while (str[i] && str1[i] && str[i] == str1[i])
		i++;
	return (str[i] - str1[i]);
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	iscommand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isalpha(str[i]))
			return (0);
		i++;
	}
	return (1);
}

t_tokenize	*ft_lstlast(t_tokenize *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

t_tokenize	*ft_lstnew(t_tokenize *prev, int cntnt)
{
	t_tokenize	*new;

	new = (t_tokenize *)malloc(sizeof(t_tokenize));
	if (!new)
		return (NULL);
	new->type = cntnt;
	new->next = NULL;
	new->prev = prev;
	return (new);
}
