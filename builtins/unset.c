/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:35:36 by rammisse          #+#    #+#             */
/*   Updated: 2025/04/30 21:30:23 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void lstremoveif(t_env **env, char *value)
{
    t_env *curr;
    t_env *prev;
    
    curr = *env;
    prev = NULL;
    while (curr)
    {
        if (!ft_strcmp(curr->value, value))
        {
            if (prev == NULL)
                *env = curr->next;
            else
                prev->next = curr->next;
            free(curr->string);
            free(curr->value);
            free(curr->variable);
            free(curr);
            if (prev == NULL)
                curr = *env;
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


void unset(char **str, t_env **env)
{
    int i;
    int j;
    t_env *tmp;

    i = 1;
    j = 0;
    while (str[i])
    {
        tmp = *env;
        while (tmp)
        {
            if (!ft_strcmp(str[i], tmp->value))
                lstremoveif(env, tmp->value);
            tmp = tmp->next;
        }
        i++;
    }
}
