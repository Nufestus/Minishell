/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:03:09 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/19 18:25:23 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalnummm(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9') || c == '_')
		return (1);
	else
		return (0);
}

int	ft_isalphaa(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return (1);
	}
	else
		return (0); 
}

char *getvalue(char *str)
{
	int i;
	char *value;
	int start;

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
	if (value)
		return (value);
	else
		return (ft_strdup(""));
}

char *getvar(char *str)
{
	int i;
	char *var;
	int start;

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

int checkvalid(char *str, t_minishell *mini)
{
	int i;
	char *var;
	
	i = 0;
	var = getvar(str);
	if (ft_isalphaa(str[i]) || str[i] == '_')
	{
		if (ft_isalphaa(str[i]) && str[i + 1] == '\0')
			return (5);
		i++;
		while (str[i] && ft_isalnummm(str[i]))
			i++;
		if (str[i] == '=' && !ft_getenv(var, &mini))
			return (free(var), 1);
		else if (str[i] == '+' && str[i + 1] == '=')
			return (free(var), 2);
		else if (str[i] == '=' && ft_getenv(var, &mini))
			return (free(var), 3);
	}
	return (free(var), 0);
}

char *getstring(char *str)
{
	int i;
	int j;
	int skip;
	char *string;

	i = 0;
	j = 0;
	skip = 0;
	while (str[i])
		i++;
	string = malloc(i + 1);
	if (!string)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (!skip && str[i] == '+' && str[i + 1] == '=')
		{
			skip = 1;
			i++; 
			continue;
		}
		string[j++] = str[i++];
	}
	string[j] = '\0';
	return (string);
}

t_env *getenvnode(t_env *env, char *var)
{
	t_env *tmp;

	tmp = env;
	while(tmp)
	{
		if (!ft_strcmp(var, tmp->variable))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void export(t_commandline *command)
{
	int i;
	t_env *new;
	size_t size;
	int start;
	char *var;
	char *value;
	char *string;
	char *tmp;
	int j;
	char **str;
	
	i = 1;
	j = 0;
	start = 0;
	size = ft_commandsize(command->mini->commandline);
	str = command->args;
	if (!str[1])
	{
		new = command->mini->env;
		if (size > 1)
		{
			while (new)
			{
				printf("declare -x %s=\"%s\"\n", new->variable, new->value);
				new = new->next;
			}
			exit(0);
		}
		else
		{
			while(new)
			{
				ft_putstr_fd("declare -x ", command->outfd);
				ft_putstr_fd(new->variable, command->outfd);
				ft_putstr_fd("=\"", command->outfd);
				ft_putstr_fd(new->value, command->outfd);
				ft_putstr_fd("\"\n", command->outfd);
				new = new->next;
			}
		}
		return ;
	}
	while (str[i])
	{
		value = getvalue(str[i]);
		var = getvar(str[i]);
		string = getstring(str[i]);
		j = 0;
		if (checkvalid(str[i], command->mini) == 1)
		{
			new = ft_envnew(value, var, string);
			new->isexported = false;
			ft_envadd_back(&command->mini->env, new);
		}
		else if (checkvalid(str[i], command->mini) == 2)
		{
			new = getenvnode(command->mini->env, var);
			if (new)
			{
				tmp = new->value;
				new->value = ft_strjoin(new->value, value);
				free(tmp);
				tmp = new->string;
				new->string = ft_strjoin(new->string, value);
				free(tmp);
			}
			else
			{
				tmp = string;
				string = getstring(string);
				free(tmp);
				new = ft_envnew(value, var, string);
				new->isexported = false;
				ft_envadd_back(&command->mini->env, new);
			}
		}
		else if (checkvalid(str[i], command->mini) == 3)
		{
			new = getenvnode(command->mini->env, var);
			tmp = new->value;
			new->value = ft_strdup(value);
			free(tmp);
			tmp = new->string;
			new->string = getstring(str[i]);
			free(tmp);
		}
		else if (checkvalid(str[i], command->mini) == 0)
			printf("export: not a valid identifier: %s\n", str[i]);
		free(value);
		free(string);
		free(var);
		i++;
	}
	if (size > 1)
		exit(0);
	return ;
}

