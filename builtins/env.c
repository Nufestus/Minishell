/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:16:10 by rammisse          #+#    #+#             */
/*   Updated: 2025/04/27 23:17:35 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void env(t_minishell *mini)
{
    t_env *tmp;

    tmp = mini->env;
    while (tmp)
    {
        printf("%s\n", tmp->variable);
        tmp = tmp->next;
    }
}
