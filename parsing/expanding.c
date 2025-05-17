/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:22:50 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/17 15:16:50 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_getenv(char *str, t_minishell **mini)
{
	t_env *temp;
	
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

int varlen(char *str, t_minishell *mini)
{
	int i;
	int insingle;
	int indouble;
	int finallen;
	int start;
	int len;
	char *var;
	char *expandedvar;

	i = 0;
	start = 0;
	len = 0;
	finallen = 0;
	insingle = 0;
	indouble = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
			{
				i++;
				start = i;
				while (str[i] && ft_isalnumm(str[i]))
					i++;
				len = i - start;
				var = ft_substr(str, start, len);
				expandedvar = ft_getenv(var, &mini);
				if (expandedvar)
					finallen += ft_strlen(expandedvar);
				free(var);
			}
			else if (str[i] == '$' && str[i + 1] == '?')
			{
				finallen += 12;
				i += 2;
			}
			else
			{
				i++;
				finallen++;
			}
		}
		else
			i++;
	}
	return (finallen);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int quoted(char *str)
{
	int i;
	int len;
	char quote;

	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			i++;
			while (str[i] && str[i] != quote)
			{
				len++;
				i++;
			}
			if (str[i] == quote)
				i++;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char *expand(int *check, char *str, t_minishell *mini)
{
    int total;
    int insingle;
    int indouble;
    int i;
	int start;
	int len;
    int j;
	char *expandedvar;
    char *expanded;
	size_t	k;
	char *var;

    total = varlen(str, mini) + ft_strlen(str);
    insingle = 0;
    indouble = 0;
    i = 0;
    j = 0;
	start = 0;
	len = 0;
	k = 0;
    expanded = malloc(total + 1);
    if (!expanded)
        return (NULL);
    while (str[i])
    {
        if (str[i] == '\'' && !indouble)
		{
            insingle = !insingle;
			i++;
			continue;
		}
        else if (str[i] == '"' && !insingle)
		{
            indouble = !indouble;
			i++;
			continue;
		}
		if (str[i] == '$' && !insingle && str[i + 1])
		{
			if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
			{
				i++;
				start = i;
				while (str[i] && ft_isalnumm(str[i]))
					i++;
				len = i - start;
				var = ft_substr(str, start, len);
				if (!var)
				{
					free(expanded);
					return(NULL);
				}
				expandedvar = ft_getenv(var, &mini);
				if (expandedvar)
				{
					if (!indouble)
					{
						if (check)
							*check = 1;
					}
					k = 0;
					while (expandedvar[k])
					{
						expanded[j] = expandedvar[k];
						j++;
						k++;
					}
				}
				free(var);
			}
			else if (str[i + 1] == '?')
			{
				i++;
				if (str[i] == '?')
					expandedvar = ft_itoa(mini->exitstatus);
				i++;
				k = 0;
				while (expandedvar[k])
				{
					expanded[j] = expandedvar[k];
					j++;
					k++;
				}
				free(expandedvar);
			}
			else if (str[i] == '$' && str[i + 1] == '$')
				i += 2;
			else if (str[i] == '$' && ft_isdigit(str[i + 1]))
				i += 2;
			else
				expanded[j++] = str[i++];
		}
		else
			expanded[j++] = str[i++];
	}
	expanded[j] = '\0';
	return (expanded);
}
