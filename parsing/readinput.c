/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readinput.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:06:43 by aammisse          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/02/25 16:20:51 by rammisse         ###   ########.fr       */
=======
/*   Updated: 2025/02/25 14:56:12 by aammisse         ###   ########.fr       */
>>>>>>> refs/remotes/origin/main
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
<<<<<<< HEAD
		tokenize(mini);
=======
		mini->commandline = ft_split(mini->input, " \t\r\n\v\f");
>>>>>>> refs/remotes/origin/main
	}
}