/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 22:59:30 by rammisse          #+#    #+#             */
/*   Updated: 2025/06/03 12:45:52 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_strdupdouble(char *cmd, char **str)
{
	int		i;
	int		j;
	char	**res;

	i = 0;
	while (str[i])
		i++;
	res = malloc(sizeof(char *) * (i + 3));
	if (!res)
		return (NULL);
	i = 0;
	if (cmd)
	{
		res[i] = ft_strdup(cmd);
		i++;
	}
	j = 0;
	while (str[j])
		res[i++] = ft_strdup(str[j++]);
	res[i] = NULL;
	return (res);
}

t_commandline	*ft_commandnew(char *cmd, char **arg)
{
	t_commandline	*new;

	new = (t_commandline *)malloc(sizeof(t_commandline));
	if (!new)
		return (NULL);
	new->args = ft_strdupdouble(cmd, arg);
	if (!cmd)
		new->cmd = NULL;
	else
		new->cmd = ft_strdup(cmd);
	new->file = NULL;
	new->infd = -2;
	new->outfd = -2;
	new->argcount = 0;
	new->pid = 0;
	new->iscmdexpand = 0;
	new->env = NULL;
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

t_files	*ft_filelast(t_files *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_files	*ft_filenew(char *file, char *del, int type)
{
	t_files	*new;

	new = (t_files *)malloc(sizeof(t_files));
	if (!new)
		return (NULL);
	new->type = type;
	new->fd = -2;
	new->hedoc = -2;
	if (file)
		new->file = ft_strdup(file);
	else
		new->file = NULL;
	if (del)
		new->delimiter = ft_strdup(del);
	else
		new->delimiter = NULL;
	new->next = NULL;
	return (new);
}
