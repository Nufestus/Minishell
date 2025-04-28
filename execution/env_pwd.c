/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:02:28 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/27 22:50:34 by aammisse         ###   ########.fr       */
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

void ft_pwd(void)
{
    char path[4096];

    if (getcwd(path, sizeof(path)))
    {
        printf("%s\n", path);
        exit(1);
    }
    else
    {
        perror("pwd");
        exit(0);
    }
}

void ft_env(t_minishell *mini, char **args)
{
    t_env *env;

    if (args[1])
    {
        ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
        exit(0);
    }
    env = mini->env;
    printf("hello world\n");
    while(env)
    {
        if (env->isexported)
            printf("%s\n", env->string);
        env = env->next;
    }
    exit(0);
}