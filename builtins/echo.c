/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:16:02 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/17 20:57:15 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static char	*handle(char *str, char *str1)
// {
// 	char	*string;

// 	if (str1 != NULL && str == NULL)
// 	{
// 		string = malloc(ft_strlen(str1) + 1);
// 		if (!string)
// 			return (NULL);
// 		ft_strlcpy(string, str1, ft_strlen(str1) + 1);
// 		return (string);
// 	}
// 	else if (str != NULL && str1 == NULL)
// 	{
// 		string = malloc(ft_strlen(str) + 1);
// 		if (!string)
// 			return (NULL);
// 		ft_strlcpy(string, str, ft_strlen(str) + 1);
// 		return (string);
// 	}
// 	else
// 		return (NULL);
// }

// static void	fillfirst(const char *s1, char *join, int i)
// {
// 	while (s1[i] != '\0')
// 	{
// 		join[i] = s1[i];
// 		i++;
// 	}
//     join[i] = 32;
//     i++;
// }

// char	*ft_join(const char *s1, const char *s2)
// {
// 	size_t	i;
// 	size_t	j;
// 	char	*join;
// 	size_t	s1len;
// 	size_t	s2len;

// 	if (!s1 || !s2)
// 		return (handle((char *)s1, (char *)s2));
// 	s1len = ft_strlen(s1);
// 	s2len = ft_strlen(s2);
// 	i = 0;
// 	j = 0;
// 	join = (char *) malloc(s1len + s2len + 2);
// 	if (join == NULL)
// 		return (NULL);
// 	fillfirst(s1, join, i);
// 	i = s1len + 1;
// 	while (s2[j] != '\0')
// 	{
// 		join[i] = s2[j];
// 		j++;
// 		i++;
// 	}
// 	join[i] = '\0';
// 	return (join);
// }

// void	helpecho(int size, char *res, int optioncheck, t_commandline *command)
// {
// 	if (size > 1)
//     {
//         if (optioncheck)
//             printf("%s\n", res);
//         else
//             printf("%s\n", res);
//     }
//     else
//     {
//         if (optioncheck)
//             ft_putstr_fd(res, command->outfd);
//         else
// 		{
// 			ft_putstr_fd(res, command->outfd);
// 			ft_putstr_fd("\n", command->outfd);
// 		}
//     }
//     if (size > 1)
// 		exit(0);
// 	command->mini->exitstatus = 0;
// }

// void initecho(int *i, char **res, int *optioncheck, int *finishedoptions)
// {
// 	*i = 1;
//     *optioncheck = 0;
// 	*finishedoptions = 0;
// 	*res = NULL;
// }

// void ft_echo(t_commandline *command)
// {
//     int i;
//     int optioncheck;
// 	int finishedoptions;
//     char *res;
//     char *copy;
//     int size;

//     size = ft_commandsize(command);
// 	initecho(&i, &res, &optioncheck, &finishedoptions);
//     copy = NULL;
//     while(command->args[i])
//     {
//         if (isanoption(command->args[i]) && !finishedoptions)
//     	    optioncheck = 1;
//     	else
//     	{
//     	    finishedoptions = 1;
//     	    res = ft_join(res, command->args[i]);
// 			if (copy)
//     	    	free(copy);
//     	    copy = res;
//     	}
// 		i++;
//     }
// 	helpecho(size, res, optioncheck, command);
//     free(copy);
//     return ;
// }

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

static char	*handle_join_malloc(const char *s1, const char *s2)
{
	char	*join;
	size_t	s1len;
	size_t	s2len;

	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	join = (char *) malloc(s1len + s2len + 2);
	return (join);
}

char	*ft_join(const char *s1, const char *s2)
{
	size_t	i;
	size_t	j;
	char	*join;

	if (!s1 || !s2)
		return (handle((char *)s1, (char *)s2));
	join = handle_join_malloc(s1, s2);
	if (join == NULL)
		return (NULL);
	i = 0;
	fillfirst(s1, join, i);
	i = ft_strlen(s1) + 1;
	j = 0;
	while (s2[j] != '\0')
	{
		join[i] = s2[j];
		j++;
		i++;
	}
	join[i] = '\0';
	return (join);
}

static void	print_result(char *res, int optioncheck, t_commandline *command)
{
	if (optioncheck)
		ft_putstr_fd(res, command->outfd);
	else
	{
		ft_putstr_fd(res, command->outfd);
		ft_putstr_fd("\n", command->outfd);
	}
	command->mini->exitstatus = 0;
}

static void	handle_stdout_result(char *res, int optioncheck)
{
	if (optioncheck)
		printf("%s", res);
	else
		printf("%s\n", res);
	exit(0);
}

void	helpecho(int size, char *res, int optioncheck, t_commandline *command)
{
	if (size > 1)
		handle_stdout_result(res, optioncheck);
	else
		print_result(res, optioncheck, command);
}

void	initecho(int *i, char **res, int *optioncheck, int *finishedoptions)
{
	*i = 0;
	*optioncheck = 0;
	*finishedoptions = 0;
	*res = NULL;
}

void	ft_echo(t_commandline *command)
{
	int		i;
	int		optioncheck;
	int		finishedoptions;
	char	*res;
	char	*copy;
	int		size;

	size = ft_commandsize(command);
	initecho(&i, &res, &optioncheck, &finishedoptions);
	copy = NULL;
	while (command->args[++i])
	{
		if (isanoption(command->args[i]) && !finishedoptions)
			optioncheck = 1;
		else
		{
			finishedoptions = 1;
			res = ft_join(res, command->args[i]);
			if (copy)
				free(copy);
			copy = res;
		}
	}
	helpecho(size, res, optioncheck, command);
	free(copy);
}
