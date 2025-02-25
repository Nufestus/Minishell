/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:06:43 by aammisse          #+#    #+#             */
/*   Updated: 2025/02/25 00:52:36 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void readinput(t_minishell *mini)
{
    while(1)
	{
		mini->input = readline(INPUT1 INPUT2);
		if (!mini->input)
		{
			free(mini->input);
			break ;
		}
		char **str = ft_split(mini->input, " \t\r\n\v\f");
		while(*str)
		{
			printf("%s -> ", *str);
			str++;
		}
		printf("NULL\n");
	}
}