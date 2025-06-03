/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:10:22 by rammisse          #+#    #+#             */
/*   Updated: 2025/06/01 20:12:41 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	checkthenext(int *seen_cmd, int *next_is_delim,
		int *next_is_file, t_tokenize **tok)
{
	if (*next_is_file)
	{
		(*tok)->type = FILE;
		*next_is_file = 0;
	}
	else if (*next_is_delim)
	{
		(*tok)->type = DEL;
		*next_is_delim = 0;
	}
	else if (!*seen_cmd)
	{
		(*tok)->type = CMD;
		*seen_cmd = 1;
	}
	else if (isanoption((*tok)->str))
		(*tok)->type = OPTION;
	else
		(*tok)->type = ARG;
	(*tok) = (*tok)->next;
}

int	retokhelp(int *seen_cmd, int *next_is_file,
		int *next_is_delim, t_tokenize **tok)
{
	if ((*tok)->type == PIPE)
	{
		*seen_cmd = 0;
		(*tok) = (*tok)->next;
		return (1);
	}
	else if ((*tok)->category && (*tok)->type != HEDOC)
	{
		*next_is_file = 1;
		(*tok) = (*tok)->next;
		return (1);
	}
	else if ((*tok)->type == HEDOC)
	{
		*next_is_delim = 1;
		(*tok) = (*tok)->next;
		return (1);
	}
	return (0);
}
