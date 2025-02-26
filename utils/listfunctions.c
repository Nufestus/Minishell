/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listfunctions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 19:33:38 by rammisse          #+#    #+#             */
/*   Updated: 2025/02/25 19:34:45 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tokenize	*ft_lstnew(int type)
{
	t_tokenize	*newnode;

	newnode = (t_tokenize *) malloc (sizeof(t_tokenize));
	if (!newnode)
		return (NULL);
	newnode->type = type;
	newnode->next = NULL;
	return (newnode);
}

t_tokenize	*ft_lstlast(t_tokenize *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_tokenize **lst, t_tokenize *new)
{
	if (!lst || !new)
		return ;
	if (*lst)
		ft_lstlast(*lst)->next = new;
	else
		*lst = new;
}
