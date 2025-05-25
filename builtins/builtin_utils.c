/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:35:30 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 01:45:03 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	atoihelp(int *overflow, long long result, long long old_result)
{
	if (old_result != result / 10)
	{
		*overflow = 1;
		return (0);
	}
	return (1);
}

long long	ft_atoi(char *str, int *overflow)
{
	int			i;
	int			sign;
	long long	old_result;
	long long	result;

	sign = 1;
	i = 0;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		old_result = result;
		result = result * 10 + str[i] - '0';
		if (!atoihelp(overflow, result, old_result))
			return (0);
		i++;
	}
	return (result * sign);
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

void	echo_loop(t_commandline *command, t_echo *echo)
{
	while (command->args[echo->i])
	{
		if (isanoption(command->args[echo->i]) && !echo->finishedoptions)
		{
			echo->optioncheck = 1;
			echo->i++;
		}
		else
		{
			echo->finishedoptions = 1;
			echo->res = ft_join(echo->res, command->args[echo->i]);
			if (echo->copy)
				free(echo->copy);
			echo->copy = echo->res;
			echo->i++;
		}
	}
}
