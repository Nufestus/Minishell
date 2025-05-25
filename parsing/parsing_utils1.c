/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:15:19 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 14:54:42 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parseoutput(t_tokenize *list)
{
	if (list->prev && list->prev->type != WORD && list->prev->type != PIPE)
		return (0);
	else if (!list->next)
		return (0);
	return (1);
}

void	retokenize(t_minishell *mini)
{
	t_tokenize	*list;

	list = mini->tokens;
	while (list)
	{
		if (list->type == ARG && list->prev
			&& (list->prev->type == FILE || list->prev->type == PIPE)
			&& ((list->next && (list->next->type == ARG
						|| list->next->category)) || !list->next))
			list->type = CMD;
		list = list->next;
	}
}

int	tokenizewordshelp(t_tokenize **list)
{
	if ((*list)->type != WORD)
	{
		(*list) = (*list)->next;
		return (1);
	}
	tokenhelper(list);
	return (0);
}

void	tokenizewords(t_minishell *mini)
{
	t_tokenize	*list;

	list = mini->tokens;
	while (list)
	{
		if (tokenizewordshelp(&list))
			continue ;
		list = list->next;
	}
}

int	isanoption(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
			i++;
	}
	if (str[i] == '\0' && i != 0)
		return (1);
	return (0);
}
