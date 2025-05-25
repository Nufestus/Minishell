/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:29:19 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/25 18:31:16 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalnummm(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9') || c == '_')
		return (1);
	return (0);
}

int	ft_isalphaa(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

char	*getvalue(char *str)
{
	int		i;
	char	*value;
	int		start;

	i = 0;
	start = 0;
	value = NULL;
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		i++;
		while (str[i] && ft_isalnummm(str[i]))
			i++;
		if (str[i] == '+' && str[i + 1] == '=')
			i += 2;
		else if (str[i] == '=')
			i++;
		start = i;
		while (str[i])
			i++;
		value = ft_substr(str, start, i - start);
	}
	return (value);
}

char	*getvar(char *str)
{
	int		i;
	char	*var;
	int		start;

	i = 0;
	start = 0;
	var = NULL;
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		start = i;
		while (str[i] && ft_isalnummm(str[i]))
			i++;
		var = ft_substr(str, start, i - start);
	}
	return (var);
}

int	checkvalid(char *str, t_minishell *mini)
{
	int		i;
	char	*var;

	i = 0;
	var = getvar(str);
	if (ft_isalphaa(str[i]) || str[i] == '_')
	{
		i++;
		while (str[i] && ft_isalnummm(str[i]))
			i++;
		if (str[i] == '=' && !ft_getenvv(var, &mini))
			return (free(var), 1);
		else if (str[i] == '+' && str[i + 1] == '=')
			return (free(var), 2);
		else if (str[i] == '=' && ft_getenvv(var, &mini))
			return (free(var), 3);
		else if (str[i] == '\0' && !ft_getenvv(var, &mini))
			return (free(var), 4);
		else if (str[i] == '\0' && ft_getenvv(var, &mini))
			return (free(var), 5);
	}
	return (free(var), 0);
}
