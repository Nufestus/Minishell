/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:04:21 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/03 00:46:20 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    setupnode(int index, int type, char *str, t_tokenize **tokens)
{
    t_tokenize *newnode;

    newnode = ft_lstnew(ft_lstlast(*tokens), type);
    newnode->index = index;
    newnode->str = str;
    ft_lstadd_back(tokens, newnode);
}

void syntax(int flag, t_minishell *mini)
{
    (void)mini;
    if (flag == 1)
        printf("syntax error: start with pipe '|'\n");
    else if (flag == 2)
        printf("syntax error: in pipe '|'\n");
    else if (flag == 3)
        printf("syntax error: in redirection '>'\n");
    else if (flag == 4)
        printf("syntax error: in redirection '<'\n");
    exit(0);
}

int parsecmd(t_tokenize *list)
{
    if (list->prev && list->prev->type != WORD)
        return (0);
    else if (list->next && list->next->type == PIPE)
        return (0);
    return (1);
}

int parseinput(t_tokenize *list)
{
    if (list->next && list->next->type != WORD)
        return (0);
    return (1);
}

int parseoutput(t_tokenize *list)
{
    if (list->prev && (list->prev->type != WORD || list->prev->type != PIPE))
        return (0);
    return (1);
}

void tokenizewords(t_minishell *mini)
{
    t_tokenize *list;

    list = mini->tokens;
    while (list)
    {
        if (list->type == PIPE && list->index == 0)
            syntax(1, mini);
        else if (list->type == PIPE && list->index != 0)
        {
            if (!parsecmd(list))
                syntax(2, mini);
        }
        else if (list->type == APPEND || list->type == REDOUT)
        {
            if (!parseinput(list))
                syntax(3, mini);
        }
        else if (list->type == HEDOC || list->type == REDIN)
        {
            if (!parseoutput(list))
                syntax(4, mini);
        }
        list = list->next;
    }
}

void tokenizesymbols(char **str, t_minishell *mini)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_strcmp(str[i], "|"))
            setupnode(i, PIPE, ft_strdup(str[i]), &mini->tokens);
        else if (!ft_strcmp(str[i], ">>"))
            setupnode(i, APPEND, ft_strdup(str[i]), &mini->tokens);
        else if (!ft_strcmp(str[i], "<<"))
            setupnode(i, HEDOC, ft_strdup(str[i]), &mini->tokens);
        else if (!ft_strcmp(str[i], ">"))
            setupnode(i, REDOUT, ft_strdup(str[i]), &mini->tokens);
        else if (!ft_strcmp(str[i], "<"))
            setupnode(i, REDIN, ft_strdup(str[i]), &mini->tokens);
        else if (str[i][0] == '-')
            setupnode(i, OPTION, ft_strdup(str[i]), &mini->tokens);
        else
            setupnode(i, WORD, ft_strdup(str[i]), &mini->tokens);
        i++;
    }
}

void tokenize(t_minishell *mini)
{
    int i;
    char **str;

    i = 0;
    mini->tokens = NULL;
    str = ft_split(mini->input, " \t\n\r\v\f");
    tokenizesymbols(str, mini);
    tokenizewords(mini);
    while(mini->tokens)
    {
        printf("%s / %d\n", mini->tokens->str, mini->tokens->type);
        mini->tokens = mini->tokens->next;
    }
    freedoublearray(str);
}
