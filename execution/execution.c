/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 00:13:35 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/27 04:32:13 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void openfiles(t_commandline *command)
{
    int outfd;
    int infd;
    t_files *infiles;
    t_files *outfiles;
    t_commandline *copy;
    
    copy = command;
    outfd = -1;
    infd = -1;
    infiles = copy->infile;
    while(infiles)
    {
        if (infiles->type != HEDOC)
            infd = open(infiles->file, O_RDONLY);
        infiles = infiles->next;
    }
    copy->infd = infd;
    outfiles = copy->outfile;
    while(outfiles)
    {
        if (outfiles->type == REDOUT)
            outfd = open(outfiles->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
        else if (outfiles->type == APPEND)
            outfd = open(outfiles->file, O_RDWR | O_CREAT | O_APPEND, 0644);
        outfiles = outfiles->next;
    }
    copy->outfd = outfd;
}


void	freestr(char **str)
{
    int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char *my_getenv(t_minishell *mini, char *str)
{
    t_env *env;
    
    env = mini->env;
    while(env)
    {
        if (!ft_strcmp(env->variable, str))
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

int	ft_find(char *str, char *del)
{
    int	i;
	int	j;
    
	i = 0;
	while (str[i])
	{
        j = 0;
		while (del[j])
		{
			if (str[i] == del[j])
				return (1);
                j++;
            }
            i++;
        }
	return (0);
}

char	*checkfile(t_commandline *command)
{
    int		i;
	char	*str;
	char	*tmp;
	char	**path;
    
	i = 0;
	tmp = my_getenv(command->mini, "PATH=");
	path = split(tmp, ":");
	i = -1;
	while (path[++i])
	{
        str = ft_strjoin(path[i], "/");
		tmp = str;
		str = ft_strjoin(str, command->args[0]);
		free(tmp);
		if (!access(str, F_OK | X_OK))
            return (freestr(path), ft_strdup(str));
		free(str);
	}
	return (freestr(path), NULL);
}

char **constructenv(t_env *env)
{
    int i; 
    int len;
    int size;
    char **res;
    t_env *copy;
    
    i = 0;
    copy = env;
    size = ft_envsize(env);
    res = malloc(sizeof(char *) * (size + 1));
    while (i < size)
    {
        len = ft_strlen(copy->string) + 1;
        res[i] = malloc(len);
        strcpy(res[i], copy->string);
        res[i][len - 1] = '\0';
        copy = copy->next;
        i++;
    }
    res[size] = NULL;
    return (res);
}

void initializepipes(t_minishell *mini)
{
    int size;
    int i;

    i = 0;
    size = ft_commandsize(mini->commandline);
    mini->pipes = malloc(size * sizeof(int *));
    while(i < size)
    {
        mini->pipes[i] = malloc(sizeof(int) * 2);
        pipe(mini->pipes[i]);
        i++;
    }
}


void handleiolast(t_commandline *command)
{
    t_minishell *mini;
    
    mini = command->mini;
    if (command->infd == -1)
        command->infd = mini->pipes[command->index - 1][0];
    if (command->outfd == -1)
        command->outfd = 1;
}

void setuplastcommand(t_commandline *command)
{
    pid_t pid;
    int size;
    t_minishell *mini;
    
    mini = command->mini;
    size = ft_commandsize(mini->commandline);
    command->env = constructenv(mini->env);
    handleiolast(command);
    pid = fork();
    if (!pid)
    {
        if (command->infd != STDIN_FILENO)
        {
            dup2(command->infd, 0);
            close(command->infd);
        }
        if (command->outfd != STDOUT_FILENO)
        {
            dup2(command->outfd, 1);
            close(command->outfd);
        }
        if (!ft_find(command->args[0], "./"))
            command->cmd = checkfile(command);
        else
            command->cmd = ft_strdup(command->args[0]);
        execve(command->cmd, command->args, command->env);
        perror(command->cmd);
        exit(0);
    }
    close(mini->pipes[command->index][1]);
    close(mini->pipes[command->index - 1][0]);
}

void handleiomiddle(t_commandline *command)
{
    t_minishell *mini;
    
    mini = command->mini;
    if (command->infd == -1)
        command->infd = mini->pipes[command->index - 1][0];
    if (command->outfd == -1)
        command->outfd = mini->pipes[command->index][1];
}

void setupmiddlecommand(t_commandline *command)
{
    pid_t pid;
    int size;
    t_minishell *mini;

    mini = command->mini;
    size = ft_commandsize(mini->commandline);
    handleiomiddle(command);
    command->env = constructenv(mini->env);
    pid = fork();
    if (!pid)
    {
        close(mini->pipes[command->index][0]);
        if (command->infd != STDIN_FILENO)
        {
            dup2(command->infd, 0);
            close(command->infd);
        }
        if (command->outfd != STDOUT_FILENO)
        {
            dup2(command->outfd, 1);
            close(command->outfd);
        }
        if (!ft_find(command->args[0], "./"))
            command->cmd = checkfile(command);
        else
            command->cmd = ft_strdup(command->args[0]);
        execve(command->cmd, command->args, command->env);
        perror(command->cmd);
        exit(0);
    }
    close(mini->pipes[command->index][1]);
    close(mini->pipes[command->index - 1][0]);
}

void handleiosingle(t_commandline *command)
{
    t_minishell *mini;

    mini = command->mini;
    if (command->infd == -1)
        command->infd = 0;
    if (command->outfd == -1 && command->next)
        command->outfd = mini->pipes[command->index][1];
    else if (command->outfd == -1 && !command->next)
        command->outfd = 1;
}

void setupfirstcommand(t_commandline *command)
{
    pid_t pid;
    int size;
    t_minishell *mini;
    
    mini = command->mini;
    size = ft_commandsize(mini->commandline);
    handleiosingle(command);
    command->env = constructenv(mini->env);
    pid = fork();
    if (!pid)
    {
        close(mini->pipes[command->index][0]);
        if (command->infd != STDIN_FILENO)
        {
            dup2(command->infd, 0);
            close(command->infd);
        }
        if (command->outfd != STDOUT_FILENO)
        {
            dup2(command->outfd, 1);
            close(command->outfd);
        }
        if (!ft_find(command->args[0], "./"))
            command->cmd = checkfile(command);
        else
            command->cmd = ft_strdup(command->args[0]);
        execve(command->cmd, command->args, command->env);
        perror(command->cmd);
        exit(0);
    }
    close(mini->pipes[command->index][1]);
}

void	childlabor(t_commandline *command)
{
    if (!command->index)
        setupfirstcommand(command);
    else if (!command->next)
        setuplastcommand(command);
    else
        setupmiddlecommand(command);
}

void	startpipex(t_commandline *command)
{
	int	i;
    int size;
    t_commandline *copy;

	i = -1;
    copy = command;
    initializepipes(command->mini);
    size = ft_commandsize(copy);
	while (++i < size)
    {
        openfiles(copy);
        childlabor(copy);
        copy = copy->next;
    }
	waitpid(-1, NULL, 0);
}

void execute(t_minishell *mini)
{
    // removenodes(mini);
    startpipex(mini->commandline);
}