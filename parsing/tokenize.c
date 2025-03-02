/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:04:21 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/02 21:08:37 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    setupnode(int type, char *str, t_tokenize **mini)
{
    t_tokenize *newnode;

    newnode = ft_lstnew(type);
    newnode->str = str;
    ft_lstadd_back(mini, newnode);
}

void tokenize(t_minishell *mini)
{
    int i;
    char **str;
    t_tokenize *head = NULL;

    i = 0;
    str = ft_split(mini->input, " \t\n\r\v\f");
    while (str[i])
    {
        if (!ft_strcmp(str[i], "|"))
            setupnode(PIPE, str[i], &head);
        else if (!ft_strcmp(str[i], ">>"))
            setupnode(REDOUTAPP, str[i], &head);
        else if (!ft_strcmp(str[i], "<<"))
            setupnode(HEDOC, str[i], &head);
        else if (!ft_strcmp(str[i], ">"))
            setupnode(REDOUT, str[i], &head);
        else if (!ft_strcmp(str[i], "<"))
            setupnode(REDIN, str[i], &head);
        else
            setupnode(WORD, str[i], &head);
        i++;
    }
}
