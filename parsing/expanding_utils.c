/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:45:41 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/23 19:48:51 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv(char *str, t_minishell **mini)
{
	t_env	*temp;

	temp = (*mini)->env;
	while (temp)
	{
		if (ft_strcmp(str, temp->variable) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

int	ft_isalnumm(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9') || c == '_')
		return (1);
	else
		return (0);
}

int	handle_variable_expansion(char *str, int *i, t_minishell *mini)
{
	int		start;
	int		len;
	char	*var;
	char	*expandedvar;

	len = 0;
	(*i)++;
	start = *i;
	while (str[*i] && ft_isalnumm(str[*i]))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	expandedvar = ft_getenv(var, &mini);
	if (expandedvar)
		len = ft_strlen(expandedvar);
	free(var);
	return (len);
}

int	handle_dollar_sign(char *str, int *i, t_minishell *mini)
{
	if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		return (handle_variable_expansion(str, i, mini));
	else if (str[*i + 1] == '?')
	{
		*i += 2;
		return (12);
	}
	else
	{
		(*i)++;
		return (1);
	}
}

int	varlen(char *str, t_minishell *mini)
{
	int	i;
	int	finallen;

	i = 0;
	finallen = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0')
			finallen += handle_dollar_sign(str, &i, mini);
		else
		{
			i++;
			finallen++;
		}
	}
	return (finallen);
}
