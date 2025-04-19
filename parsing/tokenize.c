/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:04:21 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/19 17:26:43 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    setupnode(int index, int category, int type, char *str, t_tokenize **tokens)
{
    t_tokenize *newnode;

    newnode = ft_lstnew(ft_lstlast(*tokens), type);
    newnode->index = index;
    newnode->str = ft_strdup(str);
    newnode->category = category;
    ft_lstadd_back(tokens, newnode);
}

void	insert_between(t_tokenize *before, t_tokenize *after, int type, char *str, int index)
{
	t_tokenize	*new;

	new = ft_lstnew(before, type);
	new->index = index + 1;
    new->str = ft_strdup(str);
    new->category = 0;
	new->next = after;
	if (before)
		before->next = new;
	if (after)
		after->prev = new;
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
        if (list->prev && list->prev->category && list->type == WORD)
            list->type = FILE;
        else if (((!list->prev || list->prev->type == PIPE)
            || (list->prev && list->prev->type == FILE)) && list->type == WORD)
            list->type = CMD;
        else if (list->prev && list->prev->type == CMD && list->type == WORD)
            list->type = ARG;
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
            setupnode(i, 0, PIPE, str[i], &mini->tokens);
        else if (!ft_strcmp(str[i], ">>"))
            setupnode(i, 1, APPEND, str[i], &mini->tokens);
        else if (!ft_strcmp(str[i], "<<"))
            setupnode(i, 1, HEDOC, str[i], &mini->tokens);
        else if (!ft_strcmp(str[i], ">"))
            setupnode(i, 1, REDOUT, str[i], &mini->tokens);
        else if (!ft_strcmp(str[i], "<"))
            setupnode(i, 1, REDIN, str[i], &mini->tokens);
        else if (!ft_strncmp(str[i], "-n", 2))
            setupnode(i, 0, OPTION, str[i], &mini->tokens);
        else
            setupnode(i, 0, WORD, str[i], &mini->tokens);
        i++;
    }
}

char *handletypes(int i)
{
    if (i == WORD)
        return ("WORD");
    else if (i == OPTION)
        return ("OPTION");
    else if (i == CMD)
        return ("CMD");
    else if (i == REDOUT)
        return ("REDOUT");
    else if (i == REDIN)
        return ("REDIN");
    else if (i == APPEND)
        return ("APPEND");
    else if (i == HEDOC)
        return ("HEDOC");
    else if (i == PIPE)
        return ("PIPE");
    else if (i == FILE)
        return ("FILE");
    else if (i == ARG)
        return ("ARG");
    return ("NULL");
}

bool	is_token(char c) {
	return (c == '<' || c == '>' || c == '|');
}

char	*add_spaces_around_tokens(const char *input)
{
	int		i = 0;
	int		j = 0;
	char	*out = malloc(strlen(input) * 3 + 1);

	if (!out)
		return NULL;
	while (input[i])
    {
		if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
        {
			if (j > 0 && out[j - 1] != ' ')
				out[j++] = ' ';
			out[j++] = input[i++];
			out[j++] = input[i++];
			if (input[i] && input[i] != ' ')
				out[j++] = ' ';
		}
		else if (is_token(input[i]))
        {
			if (j > 0 && out[j - 1] != ' ')
				out[j++] = ' ';
			out[j++] = input[i++];
			if (input[i] && input[i] != ' ')
				out[j++] = ' ';
		}
		else
			out[j] = input[i];
        i++;
        j++;
	}
	out[j] = '\0';
	return out;
}

void tokenize(t_minishell *mini)
{
    int i;
    t_tokenize *copy;
    char **str;
    char *addspaces;

    i = 0;
    mini->tokens = NULL;
    addspaces = add_spaces_around_tokens(mini->input);
    str = ft_split(addspaces, " \t\n\r\v\f");
    tokenizesymbols(str, mini);
    tokenizewords(mini);
    copy = mini->tokens;
    while(mini->tokens)
    {
        printf("%s / %s\n", mini->tokens->str, handletypes(mini->tokens->type));
        mini->tokens = mini->tokens->next;
    }
    freelisttokens(copy);
    freedoublearray(str);
}
