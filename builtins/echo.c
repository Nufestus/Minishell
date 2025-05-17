/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:16:02 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/17 05:02:34 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*handle(char *str, char *str1)
{
	char	*string;

	if (str1 != NULL && str == NULL)
	{
		string = malloc(ft_strlen(str1) + 1);
		if (!string)
			return (NULL);
		ft_strlcpy(string, str1, ft_strlen(str1) + 1);
		return (string);
	}
	else if (str != NULL && str1 == NULL)
	{
		string = malloc(ft_strlen(str) + 1);
		if (!string)
			return (NULL);
		ft_strlcpy(string, str, ft_strlen(str) + 1);
		return (string);
	}
	else
		return (NULL);
}

static void	fillfirst(const char *s1, char *join, int i)
{
	while (s1[i] != '\0')
	{
		join[i] = s1[i];
		i++;
	}
    join[i] = 32;
    i++;
}

char	*ft_join(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;
	char	*join;
	size_t	s1len;
	size_t	s2len;

	if (!s1 || !s2)
		return (handle((char *)s1, (char *)s2));
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	i = 0;
	j = 0;
	join = (char *) malloc(s1len + s2len + 2);
	if (join == NULL)
		return (NULL);
	fillfirst(s1, join, i);
	i = s1len + 1;
	while (s2[j] != '\0')
	{
		join[i] = s2[j];
		j++;
		i++;
	}
	join[i] = '\0';
	return (join);
}

void ft_echo(t_commandline *command)
{
    int i;
    int optioncheck;
	int finishedoptions;
    char *res;
    char *copy;
    int size;

    i = 1;
    size = ft_commandsize(command->mini->commandline);
    optioncheck = 0;
	finishedoptions = 0;
    res = NULL;
    copy = NULL;
    while(command->args[i])
    {
        if (isanoption(command->args[i]) && !finishedoptions)
    	{
    	    optioncheck = 1;
    	    i++;
    	}
    	else
    	{
    	    finishedoptions = 1;
    	    res = ft_join(res, command->args[i]);
			if (copy)
    	    	free(copy);
    	    copy = res;
    	    i++;
    	}
    }
    if (size > 1)
    {
        if (optioncheck)
            printf("%s\n", res);
        else
            printf("%s\n", res);
    }
    else
    {
        if (optioncheck)
            ft_putstr_fd(res, command->outfd);
        else
		{
			ft_putstr_fd(res, command->outfd);
			ft_putstr_fd("\n", command->outfd);
		}
    }
    free(copy);
    if (size > 1)
		exit(0);
	command->mini->exitstatus = 0;
    return ;
}