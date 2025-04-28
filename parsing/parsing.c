/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 22:18:09 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/27 19:37:07 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse(t_minishell *mini)
{
	int check;
	t_tokenize *list;
	// t_tokenize *copy;

	list = mini->tokens;
	check = 0;
	while(list)
	{
		if ((list->index == 0 && list->type == PIPE)
			|| (list->next && list->next->type == PIPE 
			&& list->type == PIPE) || (!list->next && list->type == PIPE))
			syntax(&check, "'|'", mini);
		else if (list->next && list->next->category && list->category)
			syntax(&check, list->next->str, mini);
		else if (!list->next && list->category)
			syntax(&check, "'newline'", mini);
		else if (list->prev && list->prev->category && list->type == PIPE)
			syntax(&check, "'|'", mini);
		else if (!ft_strcmp(list->str, "\\"))
			syntax(&check, "'\\'", mini);
		else if (!ft_strcmp(list->str, ";"))
			syntax(&check, "';'", mini);
		if (check == 1)
			break ;
		list = list->next;
	}
	// copy = mini->tokens;
	// while(copy)
	// {
	// 	printf("%s / %s\n", copy->str, handletypes(copy->type));
	// 	copy = copy->next;
	// }
}
