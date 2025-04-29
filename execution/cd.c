/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 22:52:01 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/28 17:16:30 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_cd(t_commandline *commandline)
{
    int size;
    t_commandline *command;
    t_minishell *mini;
    
    command = commandline;
    size = ft_commandsize(commandline);
    mini = command->mini;
    if (!command->args[1])
    {
        ft_putstr_fd("cd: insufficient arguments\n", 2);
        if (size != 1)
            exit(0);
        return ;
    }
    if (command->index == 0 && !command->next)
    {
        if (chdir(command->args[1]) == -1)
        {
            perror("cd");
            if (size != 1)
                exit(0);
        }
    }
    if (size != 1)
        exit(0);
    return ;
}

