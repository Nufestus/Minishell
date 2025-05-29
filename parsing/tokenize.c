/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:04:21 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/29 18:22:22 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_reparsehelp(t_reparse *reparse, char *str, int *check)
{
	while (str[reparse->i] && !ft_strchr(" \t\n\r\v\f", str[reparse->i]))
	{
		if (str[reparse->i] == '"' || str[reparse->i] == '\'')
		{
			reparse->q = str[reparse->i];
			reparse->token[reparse->k++] = str[reparse->i++];
			while (str[reparse->i] && str[reparse->i] != reparse->q)
				reparse->token[reparse->k++] = str[reparse->i++];
			if (str[reparse->i] == reparse->q)
				reparse->token[reparse->k++] = str[reparse->i++];
			else if (str[reparse->i] != reparse->q)
			{
				*check = 1;
				free(reparse->token);
				free(str);
				return (0);
			}
		}
		else
			reparse->token[reparse->k++] = str[reparse->i++];
	}
	reparse->token[reparse->k] = '\0';
	return (1);
}

int	ft_reparsehelp1(t_reparse *reparse, t_minishell *mini)
{
	if (reparse->prev && !ft_strcmp(reparse->prev, "<<"))
	{
		reparse->string = reparse->token;
		reparse->token = removequotes(&reparse->flag, reparse->token);
		free(reparse->string);
	}
	else
	{
		reparse->string = reparse->token;
		reparse->token = expand(&reparse->flag, reparse->token, mini);
		free(reparse->string);
	}
	if (reparse->flag == 1 && mini->expanded && reparse->token[0] == '\0')
		mini->flag = 1;
	return (0);
}

void	ft_reparsehelp2(t_reparse *reparse, char **s, t_minishell *mini)
{
	t_tokenize	*last;

	s = NULL;
	if (reparse->flag == 1 && !mini->flag)
	{
		reparse->k = 0;
		s = split(reparse->token, " \t\n\r\v\f");
		while (s[reparse->k])
			handlenodes(1, s[reparse->k++], reparse->j++, mini);
		freedoublearray(s);
	}
	if (reparse->flag == 2 || reparse->flag == 3)
		handlenodes(reparse->flag, reparse->token, reparse->j++, mini);
	else
		handlenodes(0, reparse->token, reparse->j++, mini);
	if (reparse->prev)
		free(reparse->prev);
	if (mini->flag)
	{
		last = ft_lstlast(mini->tokens);
		last->flag = 1;
	}
	reparse->prev = ft_strdup(reparse->token);
	free(reparse->token);
}

void	ft_reparse(int *check, char *str, t_minishell *mini)
{
	t_reparse	reparse;
	char		**s;

	initreparse(&reparse, str);
	while (str[reparse.i])
	{
		mini->flag = 0;
		reparse.flag = 0;
		mini->expanded = 0;
		reparse.z = countword(&reparse.copy);
		while (str[reparse.i] && ft_strchr(" \t\n\r\v\f", str[reparse.i]))
			reparse.i++;
		if (!str[reparse.i])
			break ;
		reparse.token = malloc(reparse.z + 1);
		if (!reparse.token)
			return ;
		reparse.k = 0;
		if (!ft_reparsehelp(&reparse, str, check))
			return ;
		if (ft_reparsehelp1(&reparse, mini) == 2)
			continue ;
		ft_reparsehelp2(&reparse, s, mini);
	}
	free(reparse.prev);
}

int	tokenize(t_minishell *mini)
{
	int		i;
	int		check;
	char	*addspaces;

	i = 0;
	check = 0;
	mini->tokens = NULL;
	addspaces = fillspace(mini->input);
	if (addspaces == NULL)
		return (-1);
	ft_reparse(&check, addspaces, mini);
	if (check == 1)
	{
		syntax(NULL, "'quotes'", 1);
		return (-1);
	}
	retokenize(mini);
	free(addspaces);
	return (0);
}
