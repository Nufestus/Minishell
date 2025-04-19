/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 22:18:09 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/19 22:15:41 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse(t_minishell *mini)
{
	t_tokenize *list;
    t_tokenize *copy;

    list = mini->tokens;
    while(list)
    {
        if ((list->index == 0 && list->type == PIPE)
            || (list->next && list->next->type == PIPE 
                && list->type == PIPE))
            syntax("'|'", mini);
        else if (list->next && list->next->category && list->category)
            syntax(list->next->str, mini);
        else if (!list->next && list->category)
            syntax("'newline'", mini);
        else if (list->prev && list->prev->category && list->type == PIPE)
            syntax("'|'", mini);
        else if (!ft_strcmp(list->str, "\\"))
            syntax("'\\'", mini);
        else if (!ft_strcmp(list->str, ";"))
            syntax("';'", mini);
        list = list->next;
    }
    copy = mini->tokens;
    while(copy)
    {
        printf("%s / %s\n", copy->str, handletypes(copy->type));
        copy = copy->next;
    }
}
