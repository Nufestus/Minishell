/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:04:21 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/03 19:38:15 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    setupnode(int index, int type, char *str, t_tokenize **tokens)
{
    t_tokenize *newnode;

    newnode = ft_lstnew(ft_lstlast(*tokens), type);
    newnode->index = index;
    newnode->str = ft_strdup(str);
    ft_lstadd_back(tokens, newnode);
}

void syntax(char *flag, t_minishell *mini)
{
    (void)mini;
    if (flag)
    {
        flag = ft_strjoin("shell: syntax error near unexpected token ", flag);
        printf("%s\n", flag);
        free(flag);
    }
    freelisttokens(mini->tokens);
    exit(0);
}

int parsepipe(t_tokenize *list)
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
    else if (!list->next)
        return (0);
    return (1);
}

int parseoutput(t_tokenize *list)
{
    if (list->prev && list->prev->type != WORD && list->prev->type != PIPE)
        return (0);
    else if (!list->next)
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
            syntax("\'|\'", mini);
        else if (list->type == PIPE && list->index != 0)
        {
            if (!parsepipe(list))
                syntax("\'|\'", mini);
        }
        else if (list->type == APPEND || list->type == REDOUT)
        {
            if (!parseoutput(list))
                syntax("\'newline\'", mini);
        }
        else if (list->type == HEDOC || list->type == REDIN)
        {
            if (!parseinput(list))
                syntax("\'newline\'", mini);
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
            setupnode(i, PIPE, str[i], &mini->tokens);
        else if (!ft_strcmp(str[i], ">>"))
            setupnode(i, APPEND, str[i], &mini->tokens);
        else if (!ft_strcmp(str[i], "<<"))
            setupnode(i, HEDOC, str[i], &mini->tokens);
        else if (!ft_strcmp(str[i], ">") || !ft_strcmp(str[i], ">|"))
            setupnode(i, REDOUT, str[i], &mini->tokens);
        else if (!ft_strcmp(str[i], "<"))
            setupnode(i, REDIN, str[i], &mini->tokens);
        else if (str[i][0] == '-')
            setupnode(i, OPTION, str[i], &mini->tokens);
        else
            setupnode(i, WORD, str[i], &mini->tokens);
        i++;
    }
}

void tokenize(t_minishell *mini)
{
    int i;
    t_tokenize *copy;
    char **str;

    i = 0;
    mini->tokens = NULL;
    str = ft_split(mini->input, " \t\n\r\v\f");
    tokenizesymbols(str, mini);
    tokenizewords(mini);
    copy = mini->tokens;
    while(mini->tokens)
    {
        printf("%s / %d\n", mini->tokens->str, mini->tokens->type);
        mini->tokens = mini->tokens->next;
    }
    freelisttokens(copy);
    freedoublearray(str);
}
