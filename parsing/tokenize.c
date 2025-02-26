/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:04:21 by aammisse          #+#    #+#             */
/*   Updated: 2025/02/25 19:46:50 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle(unsigned char c, unsigned char b)
{
	if (c - b > 0)
		return (1);
	else if (c - b < 0)
		return (-1);
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	size_t			i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return (handle(s1[i], s2[i]));
		i++;
	}
	return (0);
}

void setupnode(t_tokenize **mini, int type, char *str)
{
	t_tokenize *newnode;

	newnode = ft_lstnew(type);
	newnode->string = str;
	ft_lstadd_back(mini, newnode);
}

void tokenize(t_minishell *mini)
{
	int		i;
	char	**str;
	t_tokenize *head;

	i = 0;
	str = ft_split(mini->input, " \t\v\n\f\r");
	while(str[i])
	{
		if (ft_strcmp(str[i], "|"))
			setupnode(&head, PIPE,str[i]);
		else if (ft_strcmp(str[i], "<<"))
			setupnode(&head, HEDOC, str[i]);
		else if (ft_strcmp(str[i], ">>"))
			setupnode(&head, APPEND, str[i]);
		else if (ft_strcmp(str[i], ">"))
			setupnode(&head, REDOUT, str[i]);
		else if (ft_strcmp(str[i], "<"))
			setupnode(&head, REDIN, str[i]);
		else
			setupnode(&head, WORD, str[i]);
		i++;
	}
}
