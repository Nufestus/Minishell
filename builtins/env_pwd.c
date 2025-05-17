/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:02:28 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/17 21:00:45 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	if (!s)
		return ;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void ft_pwd(t_minishell *mini)
{
    char *path;
    int size;

    path = NULL;
    size = ft_commandsize(mini->commandline);
    path = getcwd(NULL, 0);
    if (size != 1)
    {
        if (path)
            printf("%s\n", path);
        else
            perror("pwd");
        
    }
    else
    {
        if (path)
        {
            ft_putstr_fd(path, mini->commandline->outfd);
            ft_putstr_fd("\n", mini->commandline->outfd);
        }
        else
            perror("pwd");
    }
    if (path)
        free(path);
    if (size != 1)
        exit(0);
    mini->exitstatus = 0;
}

void printenv()
{
    
}

void ft_env(t_minishell *mini, char **args)
{
    t_env *env;
    int size;

    size = ft_commandsize(mini->commandline);
    if (args[1])
    {
        ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
        if (size != 1)
            exit(1);
        return ;
    }
    env = mini->env;
    if (size > 1)
    {
        while(env)
        {
            printf("%s\n", env->string);
            env = env->next;
        }
    }
    else
    {
        while(env)
        {
            ft_putstr_fd(env->string, mini->commandline->outfd);
            ft_putstr_fd("\n", mini->commandline->outfd);
            env = env->next;
        }
    }
    if (mini->commandline->infd != STDIN_FILENO)
        close(mini->commandline->infd);
    if (mini->commandline->outfd != STDOUT_FILENO)
        close(mini->commandline->outfd);
    if (size > 1)
        exit(0);
    return ;
}
