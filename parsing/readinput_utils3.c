/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:29:52 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/24 00:38:30 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredocsnorm(t_tokenize **list, int *count, int *check)
{
	if (((*list)->index == 0 && (*list)->type == PIPE)
		|| ((*list)->next && (*list)->next->type == PIPE
			&& (*list)->type == PIPE)
		|| (!(*list)->next && (*list)->type == PIPE))
		syntax(check, "'|'", 0);
	else if ((*list)->next && (*list)->next->category && (*list)->category)
		syntaxhere(check, (*list)->next->str, 0);
	else if (!(*list)->next && (*list)->category)
		syntax(check, "'newline'", 0);
	else if ((*list)->prev && (*list)->prev->category && (*list)->type == PIPE)
		syntax(check, "'|'", 0);
	else if (!ft_strcmp((*list)->str, "\\"))
		syntax(check, "'\\'", 0);
	else if (!ft_strcmp((*list)->str, ";"))
		syntax(check, "';'", 0);
	else if ((*list)->type == HEDOC && (*list)->next
		&& (*list)->next->type == DEL)
		(*count)++;
}

void	checkheredocs(t_minishell *mini)
{
	int			count;
	int			check;
	t_tokenize	*list;

	check = 0;
	count = 0;
	list = mini->tokens;
	while (list)
	{
		heredocsnorm(&list, &count, &check);
		if (check == 1)
			break ;
		list = list->next;
	}
	if (count > 16 && check)
	{
		mini->check = 2;
		printf("shell: maximum here-document count exceeded\n");
	}
}
