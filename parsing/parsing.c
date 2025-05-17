/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 22:18:09 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/17 03:24:12 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int countheredocs(t_tokenize *tokens)
{
	int count;
	t_tokenize *token;

	count = 0;
	token = tokens;
	while(token)
	{
		if (token->type == HEDOC && token->next && token->next->type == DEL)
			count++;
		token = token->next;
	}
	if (count > 16)
		return (0);
	return (1);
}

void	reparse(t_minishell *mini)
{
	int check;
	char *del;
	t_tokenize *list;

	check = 0;
	if (mini->check)
	{
		list = mini->tokens;
		while(list)
		{
			if ((list->index == 0 && list->type == PIPE)
				|| (list->next && list->next->type == PIPE 
				&& list->type == PIPE) || (!list->next && list->type == PIPE))
				syntax(&check, "'|'", 0);
			else if (list->next && list->next->category && list->category)
				syntaxhere(&check, list->next->str, 0);
			else if (!list->next && list->category)
				syntax(&check, "'newline'", 0);
			else if (list->prev && list->prev->category && list->type == PIPE)
				syntax(&check, "'|'", 0);
			else if (!ft_strcmp(list->str, "\\"))
				syntax(&check, "'\\'", 0);
			else if (!ft_strcmp(list->str, ";"))
				syntax(&check, "';'", 0);
			else if (list->type == HEDOC && list->next && list->next->type == DEL)
			{
				del = ft_strjoin(list->next->str, "\n");
				close(getinput(del, NULL));
				free(del);
			}
			if (check == 1)
				break ;
			list = list->next;
		}
	}
}

void	parse(t_minishell *mini)
{
	char *del;
	int check;
	t_tokenize *list;
	t_tokenize *copy;

	check = 0;
	del = NULL;
	list = mini->tokens;
	while(list)
	{
		if ((list->index == 0 && list->type == PIPE)
			|| (list->next && list->next->type == PIPE 
			&& list->type == PIPE) || (!list->next && list->type == PIPE))
			syntax(&check, "'|'", 1);
		else if (list->next && list->next->category && list->category)
			syntaxhere(&check, list->next->str, 1);
		else if (!list->next && list->category)
			syntax(&check, "'newline'", 1);
		else if (list->prev && list->prev->category && list->type == PIPE)
			syntax(&check, "'|'", 1);
		else if (!ft_strcmp(list->str, "\\"))
			syntax(&check, "'\\'", 1);
		else if (!ft_strcmp(list->str, ";"))
			syntax(&check, "';'", 1);
		if (check == 1)
			break ;
		list = list->next;
	}
	if (check == 1)
		mini->check = 1;
	copy = mini->tokens;
	while(copy)
	{
		// printf("%d\n", copy->inquotes);
		printf("%s / %s\n", copy->str, handletypes(copy->type));
		copy = copy->next;
	}
}
