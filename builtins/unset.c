/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:35:36 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/18 20:22:18 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void lstremoveif(t_env *env, char *value)
{
    t_env *curr;
    t_env *copy;
    t_env *prev;
    
    curr = env;
    prev = NULL;
    while (curr)
    {
        if (!ft_strcmp(curr->variable, value))
        {
            if (prev == NULL)
                copy = curr->next;
            else
                prev->next = curr->next;
            free(curr->string);
            free(curr->value);
            free(curr->variable);
            free(curr);
            if (prev == NULL)
                curr = copy;
            else
                curr = prev->next;
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

void unset(t_commandline *commandline)
{
    int i;
    char **str;

    i = 1;
    str = commandline->args;
    while (str[i])
    {
        lstremoveif(commandline->mini->env, str[i]);
        i++;
    }
}
