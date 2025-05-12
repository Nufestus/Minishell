/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:03:09 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/12 20:36:01 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*ft_lstlastt(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_backk(t_env **lst, t_env *new)
{
	if (!lst || !new)
		return ;
	if (*lst)
		ft_lstlast(*lst)->next = new;
	else
		*lst = new;
}

t_env	*ft_lstneww(char *string, char *variable, char *value)
{
	t_env	*newnode;

	newnode = (t_tokenize *) malloc (sizeof(t_tokenize));
	if (!newnode)
		return (NULL);
	newnode->string = string;
	newnode->value = value;
	newnode->variable = variable;
	newnode->next = NULL;
	return (newnode);
}

int	ft_isalnummm(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9') || c == '_')
		return (1);
	else
		return (0);
}

int	ft_isalphaa(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return (1);
	}
	else
		return (0); 
}

int checkvalid(char *str)
{
	int i;
	
	i = 0;
	if (ft_isalphaa(str[i]) || str[i] == '_')
	{
		i++;
		while (str[i] && ft_isalnummm(str[i]))
			i++;
		if (str[i] == '=')
			return (1);
		else if (str[i] == '\0')
			return (2);
		else 
			return (0);
	}
	else
		return (0);
}

int countvar(char *str)
{
	int i;

	i = 0;
	if (ft_isalpha(str[i] || str[i] == '_'))
	{
		i++;
		while (str[i] && ft_isalnummm(str[i]))
			i++;
	}
	return (i);
}

void export(t_minishell *mini)
{
	int i;
	char *var;
	char *value;
	char *string;
	int j;
	char **str;
	
	i = 0;
	j = 0;
	str = mini->commandline->args;
	while (str[i])
	{
		j = 0;
		if (checkvalid(str[i]) == 1)
		{
			
		}
		i++;
	}
}