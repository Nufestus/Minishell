/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_built.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:42:18 by rammisse          #+#    #+#             */
/*   Updated: 2025/06/02 11:21:13 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle(char *str, char *str1)
{
	if (str1 != NULL && str == NULL)
		return (ft_strdup(str1));
	else if (str != NULL && str1 == NULL)
		return (ft_strdup(str));
	else
		return (NULL);
}

void	checkset(char **res, int *check)
{
	*check = 0;
	if (!*res)
	{
		*res = ft_strdup("");
		*check = 1;
	}
}

void	envhelp(int size, t_minishell *mini)
{
	if (mini->commandline->infd != STDIN_FILENO)
		close(mini->commandline->infd);
	if (mini->commandline->outfd != STDOUT_FILENO)
		close(mini->commandline->outfd);
	if (size > 1)
		safe_exit(0);
}

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	if (!s)
		return ;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}
