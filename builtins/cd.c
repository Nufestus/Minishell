/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 22:52:01 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/23 20:01:47 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_setenv(char *envname, char *newvalue, t_minishell *mini)
{
    t_env *env;
    char *fullstring;
    char *copy;

    env = mini->env;
    while(env)
    {
        if (!ft_strcmp(env->variable, envname))
        {
            if (env->value)
                free(env->value);
            env->value = ft_strdup(newvalue);
            fullstring = ft_strjoin(env->variable, "=");
            copy = fullstring;
            fullstring = ft_strjoin(fullstring, env->value);
            free(copy);
            free(env->string);
            env->string = ft_strdup(fullstring);
            free(fullstring);
            return ;
        }
        env = env->next;
    }
}

void ft_cd(t_commandline *commandline)
{
    int size;
    char *targetdir;
    t_commandline *command;
    t_minishell *mini;
    char *oldcwd;
    char *pwd;
    
    command = commandline;
    size = ft_commandsize(commandline->mini->commandline);
    mini = command->mini;
    targetdir = command->args[1];
    oldcwd = getcwd(0, 0);
    if (!oldcwd)
    {
        perror("getcwd");
        if (size != 1)
            exit(1);
        return ;
    }
    if (command->argcount > 1)
    {
        ft_putstr_fd("cd: too many arguments\n", 2);
        if (size != 1)
        {
            free(oldcwd);
            exit(1);
        }
        return ;
    }
    if (!targetdir)
    {
        targetdir = ft_getenv("HOME", &mini);
        if (!targetdir)
        {
            ft_putstr_fd("cd; HOME not set!", 2);
            if (size != 1)
            {
                free(oldcwd);
                exit(1);
            }
            return ;
        }
    }
    if (chdir(targetdir) == -1)
    {
        perror("cd");
        if (size != 1)
        {
            free(oldcwd);
            exit(1);
        }
        return ;
    }
    pwd = getcwd(0, 0);
    if (!pwd)
    {
        perror("getcwd");
        if (size != 1)
        {
            free(oldcwd);
            free(pwd);
            exit(1);
        }
        return ;
    }
    ft_setenv("PWD", pwd, mini);
    ft_setenv("OLDPWD", oldcwd, mini);
    free(oldcwd);
    free(pwd);
    if (size != 1)
        exit(0);
    command->mini->exitstatus = 0;
    return ;
}

