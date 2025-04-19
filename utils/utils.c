/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:48:34 by rammisse          #+#    #+#             */
/*   Updated: 2025/04/19 22:45:28 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_strcmp(char *str, char *str1)
{
    while (*str && *str1 && *str == *str1)
	{
		str++;
		str1++;
	}
	return (*str - *str1);
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return (1);
	}
	else
		return (0);
}

int iscommand(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isalpha(str[i]))
            return (0);
        i++;
    }
    return (1);
}

t_tokenize	*ft_lstlast(t_tokenize *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

t_tokenize	*ft_lstnew(t_tokenize *prev, int cntnt)
{
	t_tokenize	*new;

	new = (t_tokenize *)malloc(sizeof(t_tokenize));
	if (!new)
		return (NULL);
	new->type = cntnt;
	new->next = NULL;
	new->prev = prev;
	return (new);
}

void	ft_lstadd_back(t_tokenize **lst, t_tokenize *new)
{
	if (!lst || !new)
		return ;
	if (*lst)
		ft_lstlast(*lst)->next = new;
	else
		*lst = new;
}

char	*ft_strdup(char *s1)
{
	size_t	i;
	char	*copy;
	char	*str;

	i = 0;
	str = (char *)s1;
	copy = (char *) malloc(strlen(str) + 1);
	if (copy == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

void	freedoublearray(char **str)
{
	int i;

	i = 0;
	while(str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void freelisttokens(t_tokenize *list)
{
	t_tokenize *copy;

	while (list)
	{
		copy = list->next;
		free(list->str);
		free(list);
		list = copy;
	}
}

static int	handle(unsigned char c, unsigned char b)
{
	if (c - b > 0)
		return (1);
	else if (c - b < 0)
		return (-1);
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((str1[i] != '\0' || str2[i] != '\0') && i < n)
	{
		if (str1[i] != str2[i])
			return (handle(str1[i], str2[i]));
		i++;
	}
	return (0);
}

t_commandline	*ft_commandlast(t_commandline *lst)
{
	if (!lst)
		return (NULL);
	if (!lst->next)
		return (lst);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_commandline	*ft_commandnew(char *cmd, char *option, char *arg, int args)
{
	char *str;
	char *copy;
	t_commandline	*new;

	new = (t_commandline *)malloc(sizeof(t_commandline));
	if (!new)
		return (NULL);
	str = ft_strjoin(cmd, " ");
	copy = str;
	str = ft_strjoin(str, option);
	free(copy);
	copy = str;
	str = ft_strjoin(str, " ");
	free(copy);
	copy = str;
	str = ft_strjoin(str, arg);
	free(copy);
	new->args = ft_split(str, " ");
	new->numargs = args;
	new->cmd = ft_strdup(cmd);
	new->next = NULL;
	return (new);
}

void	ft_commandadd_back(t_commandline **lst, t_commandline *new)
{
	if (!lst || !new)
		return ;
	if (*lst)
		ft_commandlast(*lst)->next = new;
	else
		*lst = new;
}
