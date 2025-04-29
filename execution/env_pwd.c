/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:02:28 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/28 17:59:00 by aammisse         ###   ########.fr       */
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
    char path[4096];
    int size;

    size = ft_commandsize(mini->commandline);
    if (getcwd(path, sizeof(path)))
        printf("%s\n", path);
    else
        perror("pwd");
    if (size != 1)
        exit(0);
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
            exit(0);
        return ;
    }
    env = mini->env;
    if (size > 1)
    {
        while(env)
        {
            if (env->isexported)
                printf("%s\n", env->string);
            env = env->next;
        }
    }
    else
    {
        while(env)
        {
            if (env->isexported)
            {
                ft_putstr_fd(env->string, mini->commandline->outfd);
                ft_putstr_fd("\n", mini->commandline->outfd);
            }
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