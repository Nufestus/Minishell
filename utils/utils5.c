/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 23:01:53 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 19:31:15 by rammisse         ###   ########.fr       */
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

void	directory_free(t_commandline *command)
{
	char *str;

	str = ft_strdup(command->cmd);
	freedoubleint(command->mini);
	freelistcommandline(command->mini->commandline);
	directoryerror(str);
}
