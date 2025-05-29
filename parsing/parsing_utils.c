/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:14:36 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/29 13:36:53 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setupnode(t_setupnode *setup,
		int category, int type, t_tokenize **tokens)
{
	t_tokenize	*newnode;

	newnode = ft_lstnew(ft_lstlast(*tokens), type);
	newnode->split = setup->split;
	newnode->flag = 0;
	newnode->index = setup->index;
	newnode->str = ft_strdup(setup->str);
	newnode->category = category;
	ft_lstadd_back(tokens, newnode);
}

void	syntaxhere(int *check, char *flag, int print)
{
	if (print)
	{
		if (flag)
			printf("shell: syntax error near unexpected token '%s'\n", flag);
	}
	if (check)
		*check = 1;
}

void	syntax(int *check, char *flag, int print)
{
	char	*error;

	error = "shell: syntax error near unexpected token ";
	if (print)
	{
		if (flag)
		{
			flag = ft_strjoin(error, flag);
			printf("%s\n", flag);
			free(flag);
		}
	}
	if (check)
		*check = 1;
}

int	parsepipe(t_tokenize *list)
{
	if (list->prev && list->prev->type != WORD)
		return (0);
	else if (list->next && list->next->type == PIPE)
		return (0);
	return (1);
}

int	parseinput(t_tokenize *list)
{
	if (list->next && list->next->type != WORD)
		return (0);
	else if (!list->next)
		return (0);
	return (1);
}
