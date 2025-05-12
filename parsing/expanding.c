/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:22:50 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/03 16:06:53 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_getenv(char *str, t_minishell *mini)
{
	t_env *temp;
	
	temp = mini->env;
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

int    lenwithoutvar(char *str)
{   
	int        i;
	int        j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			i++;
			if (ft_isalpha(str[i]) || str[i] == '_')
			{
				while (str[i] && ft_isalnumm(str[i]))
					i++;
			}
			else 
			{
				j++;
				if (str[i])
				{
					j++;
					i++;
				}
			}
		}
		else
		{
			j++;
			i++;
		}
	}
	return (j);
}

int varlen(char *str, t_minishell *mini)
{
	int i;
	int finallen;
	int start;
	int len;
	char *var;
	char *expandedvar;

	i = 0;
	start = 0;
	len = 0;
	finallen = 0;
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
				expandedvar = ft_getenv(var, mini);
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
				i++;
		}
		else
			i++;
	}
	return (finallen);
}

char *expand(char *str, t_minishell *mini)
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

    total = varlen(str, mini) + lenwithoutvar(str);
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
				expandedvar = ft_getenv(var, mini);
				if (expandedvar)
				{
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
			else
				expanded[j++] = str[i++];
		}
		else
			expanded[j++] = str[i++];
	}
	expanded[j] = '\0';
	return (expanded);
}

char **expanding(char **strs, t_minishell *mini)
{
	int		i;
	char	*tmp;
	
	i = 0;
	while (strs[i])
	{
		tmp = strs[i];
		strs[i] = expand(strs[i], mini);
		free(tmp);
		i++;
	}
	return (strs);
}
