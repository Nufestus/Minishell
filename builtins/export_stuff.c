/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_stuff.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 20:12:20 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/27 20:15:26 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exportsorted(t_commandline *command)
{
	t_env	*env;
	int		i;

	env = command->mini->env;
	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char	**tosortarray(t_commandline *command)
{
	char	**str;
	t_env	*env;
	int		i;
	int		count;

	count = exportsorted(command);
	str = malloc(sizeof(char *) * (count + 1));
	if (!str)
		return (NULL);
	env = command->mini->env;
	i = 0;
	while (env && i < count)
	{
		if (ft_strcmp("_", env->variable) != 0)
		{
			str[i] = malloc(ft_strlen(env->string) + 1);
			if (!str[i])
				return (NULL);
			ft_strlcpy(str[i], env->string, ft_strlen(env->string) + 1);
			i++;
		}
		env = env->next;
	}
	str[i] = NULL;
	return (str);
}

char	**sortarray(char **str)
{
	char		*string;
	int			count;
	int			j;
	int			i;

	i = -1;
	count = 0;
	while (str[count])
		count++;
	while (++i < count)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(str[j], str[j + 1]) > 0)
			{
				string = str[j];
				str[j] = str[j + 1];
				str[j + 1] = string;
			}
			j++;
		}
	}
	return (str);
}

int	printarrayhelp(char **array, int *i, int *j)
{
	if (array[*i][*j] == '=')
	{
		write (1, &array[*i][*j], 1);
		(*j)++;
		write (1, "\"", 1);
		while (array[*i][*j])
			write (1, &array[*i][(*j)++], 1);
		write (1, "\"", 1);
		return (1);
	}
	else
	{
		write (1, &array[*i][*j], 1);
		(*j)++;
	}
	return (0);
}

void	printarray(char **array)
{
	int		i;
	int		j;

	j = 0;
	i = 0;
	while (array[i])
	{
		write(1, "declare -x ", 11);
		j = 0;
		while (array[i][j])
		{
			if (printarrayhelp(array, &i, &j))
				break ;
		}
		write(1, "\n", 1);
		i++;
	}
}
