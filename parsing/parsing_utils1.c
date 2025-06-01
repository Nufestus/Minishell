/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:15:19 by rammisse          #+#    #+#             */
/*   Updated: 2025/06/01 20:09:14 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parseoutput(t_tokenize *list)
{
	if (list->prev && list->prev->type != WORD && list->prev->type != PIPE)
		return (0);
	else if (!list->next)
		return (0);
	return (1);
}

void	retokenizehelp(t_tokenize *tok, t_tokenexpect *token)
{
	if (tok->type == PIPE)
	{
		token->expect_cmd = 1;
		token->expect_file = 0;
		token->expect_delim = 0;
	}
	else if (tok->category && tok->type != HEDOC)
	{
		token->expect_file = 1;
		token->expect_cmd = 0;
		token->expect_delim = 0;
	}
	else if (tok->type == HEDOC)
	{
		token->expect_delim = 1;
		token->expect_file = 0;
		token->expect_cmd = 0;
	}
}

void	retokenizehelp2(t_tokenize **tok, t_tokenexpect *token)
{
	if (token->expect_delim)
	{
		(*tok)->type = DEL;
		token->expect_delim = 0;
		token->expect_cmd = 1;
	}
	else if (token->expect_file)
	{
		(*tok)->type = FILE;
		token->expect_file = 0;
		token->expect_cmd = 1;
	}
	else if (token->expect_cmd)
	{
		(*tok)->type = CMD;
		token->expect_cmd = 0;
	}
	else if (isanoption((*tok)->str))
		(*tok)->type = OPTION;
	else
		(*tok)->type = ARG;
}

void	retokenize(t_minishell *mini)
{
	t_tokenize	*tok;
	int			seen_cmd;
	int			next_is_file;
	int			next_is_delim;

	seen_cmd = 0;
	next_is_file = 0;
	next_is_delim = 0;
	tok = mini->tokens;
	while (tok)
	{
		if (retokhelp(&seen_cmd, &next_is_file, &next_is_delim, &tok))
			continue ;
		else if (tok->type == WORD)
		{
			checkthenext(&seen_cmd, &next_is_delim, &next_is_file, &tok);
			continue ;
		}
		else
		{
			tok = tok->next;
			continue ;
		}
	}
}

int	isanoption(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i] && ft_isalpha(str[i]))
			i++;
	}
	if (str[i] == '\0' && i != 1 && i != 0)
		return (1);
	return (0);
}
