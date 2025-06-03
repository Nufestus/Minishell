/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:28:50 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/28 12:55:18 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handlefileshelp(t_handlefiles *files, t_tokenize **token)
{
	if (((*token)->next->split == 2 || (*token)->next->split == 3)
		&& (*token)->type == HEDOC)
		(*token)->split = (*token)->next->split;
	if ((*token)->type == REDIN || (*token)->type == APPEND
		|| (*token)->type == REDOUT)
	{
		files->type = (*token)->type;
		files->file = ft_strdup((*token)->next->str);
	}
	else if ((*token)->type == HEDOC)
	{
		if ((*token)->split == 2 || (*token)->split == 3)
			files->delquotes = 1;
		files->type = (*token)->type;
		files->del = ft_strdup((*token)->next->str);
	}
}

void	helperhandle(t_tokenize *token, t_handlefiles *files)
{
	if (token->type == REDOUT || token->type == APPEND)
		files->new->redir = OUT;
}

void	freeextra(char *str, char *s)
{
	setexit(130, 0);
	if (s)
		free(s);
	if (str)
		free(str);
}

int	handlefiles(t_tokenize *token, t_commandline *command)
{
	t_handlefiles	files;

	files.type = 0;
	files.file = NULL;
	files.del = NULL;
	files.new = NULL;
	files.list = NULL;
	files.delquotes = 0;
	handlefileshelp(&files, &token);
	files.new = ft_filenew(files.file, files.del, files.type);
	files.new->delinquotes = files.delquotes;
	ft_fileadd_back(&command->file, files.new);
	if (token->type == REDIN || token->type == HEDOC)
	{
		if (token->type == HEDOC)
		{
			files.new->hedoc = getinput(files.new->delinquotes,
					files.del, command->mini);
			if (files.new->hedoc == -4)
				return (freeextra(files.file, files.del), 1);
		}
		files.new->redir = IN;
	}
	helperhandle(token, &files);
	return (free(files.file), free(files.del), 0);
}
