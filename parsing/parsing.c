/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 22:18:09 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/31 15:24:43 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	countheredocs(t_tokenize *tokens)
{
	int			count;
	t_tokenize	*token;

	count = 0;
	token = tokens;
	while (token)
	{
		if (token->type == HEDOC && token->next && token->next->type == DEL)
			count++;
		token = token->next;
	}
	if (count > 16)
		return (0);
	return (1);
}

void	reparsehelp(t_tokenize *list, int *check, t_minishell *mini, char *del)
{
	if ((list->index == 0 && list->type == PIPE)
		|| (list->next && list->next->type == PIPE
			&& list->type == PIPE) || (!list->next && list->type == PIPE))
		syntax(check, "'|'", 0);
	else if (list->next && list->next->category && list->category)
		syntaxhere(check, list->next->str, 0);
	else if (!list->next && list->category)
		syntax(check, "'newline'", 0);
	else if (list->prev && list->prev->category && list->type == PIPE)
		syntax(check, "'|'", 0);
	else if (!ft_strcmp(list->str, "\\"))
		syntax(check, "'\\'", 0);
	else if (!ft_strcmp(list->str, ";"))
		syntax(check, "';'", 0);
	else if (list->type == HEDOC && list->next && list->next->type == DEL)
	{
		del = ft_strdup(list->next->str);
		close(getinput(0, del, mini));
		free(del);
	}
}

void	reparse(t_minishell *mini)
{
	int			check;
	char		*del;
	t_tokenize	*list;

	check = 0;
	del = NULL;
	if (mini->check)
	{
		list = mini->tokens;
		while (list)
		{
			reparsehelp(list, &check, mini, del);
			if (check == 1)
				break ;
			list = list->next;
		}
	}
}

void	parsehelp(t_tokenize *list, int *check)
{
	if ((list->index == 0 && list->type == PIPE)
		|| (list->next && list->next->type == PIPE
			&& list->type == PIPE) || (!list->next && list->type == PIPE))
		syntax(check, "'|'", 1);
	else if (list->next && list->next->category && list->category)
		syntaxhere(check, list->next->str, 1);
	else if (!list->next && list->category)
		syntax(check, "'newline'", 1);
	else if (list->prev && list->prev->category && list->type == PIPE)
		syntax(check, "'|'", 1);
	else if (!ft_strcmp(list->str, "\\"))
		syntax(check, "'\\'", 1);
	else if (!ft_strcmp(list->str, ";"))
		syntax(check, "';'", 1);
}

void	parse(t_minishell *mini)
{
	int			check;
	t_tokenize	*list;

	check = 0;
	list = mini->tokens;
	while (list)
	{
		parsehelp(list, &check);
		if (check == 1)
			break ;
		list = list->next;
	}
	if (check == 1)
	{
		setexit(2, 0);
		mini->check = 1;
	}
}
