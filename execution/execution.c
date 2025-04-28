/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 00:13:35 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/28 14:21:37 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void openfiles(t_commandline *command)
{
    int outfd;
    int infd;
    t_files *infiles;
    t_files *outfiles;
    
    outfd = -2;
    infd = -2;
    infiles = command->infile;
    while(infiles)
    {
        if (infiles->type != HEDOC)
            infd = open(infiles->file, O_RDONLY);
        if (infd == -1)
        {
            perror(infiles->file);
            exit(0);
        }
        infiles = infiles->next;
    }
    command->infd = infd;
    outfiles = command->outfile;
    while(outfiles)
    {
        if (outfiles->type == REDOUT)
            outfd = open(outfiles->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
        else if (outfiles->type == APPEND)
            outfd = open(outfiles->file, O_RDWR | O_CREAT | O_APPEND, 0644);
        outfiles = outfiles->next;
    }
    command->outfd = outfd;
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
	tmp = my_getenv(command->mini, "PATH");
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
	return (freestr(path), ft_strdup(command->args[0]));
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

void	printerror(char *str)
{
	write(2, str, ft_strlen(str));
	write(2, ": Permission denied\n", 21);
	exit(EXIT_FAILURE);
}

void	error(char *str)
{
	char	*tmp;

	tmp = ft_strjoin(str, ": command not found\n");
	write(2, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

void handlebuiltins(t_commandline *command)
{
    if (!ft_strcmp(command->cmd, "env"))
        ft_env(command->mini, command->args);
    else if (!ft_strcmp(command->cmd, "pwd"))
        ft_pwd();
    else if (!ft_strcmp(command->cmd, "cd"))
        ft_cd(command);
}

void handleiolast(t_commandline *command)
{
    t_minishell *mini;
    
    mini = command->mini;
    if (command->infd == -2)
        command->infd = mini->pipes[command->index - 1][0];
    if (command->outfd == -2)
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
    pid = fork();
    if (!pid)
    {
        openfiles(command);
        handleiolast(command);
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
        if (!ft_find(command->args[0], "./") && !checkcommand(command))
        {
            free(command->cmd);
            command->cmd = checkfile(command);
        }
        else
        {
            free(command->cmd);
            command->cmd = ft_strdup(command->args[0]);
        }
        if (access(command->cmd, F_OK) == 0 && access(command->cmd, X_OK) == -1)
            printerror(command->cmd);
        handlebuiltins(command);
        execve(command->cmd, command->args, command->env);
        if (!ft_find(command->args[0], "./"))
			error(command->args[0]);
		else
			perror(command->args[0]);
        exit(EXIT_FAILURE);
    }
    close(mini->pipes[command->index][1]);
    close(mini->pipes[command->index - 1][0]);
}

void handleiomiddle(t_commandline *command)
{
    t_minishell *mini;
    
    mini = command->mini;
    if (command->infd == -2)
        command->infd = mini->pipes[command->index - 1][0];
    if (command->outfd == -2)
        command->outfd = mini->pipes[command->index][1];
}

void setupmiddlecommand(t_commandline *command)
{
    pid_t pid;
    int size;
    t_minishell *mini;

    mini = command->mini;
    size = ft_commandsize(mini->commandline);
    command->env = constructenv(mini->env);
    pid = fork();
    if (!pid)
    {
        openfiles(command);
        handleiomiddle(command);
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
        if (!ft_find(command->args[0], "./") && !checkcommand(command))
        {
            free(command->cmd);
            command->cmd = checkfile(command);
        }
        else
        {
            free(command->cmd);
            command->cmd = ft_strdup(command->args[0]);
        }
        if (access(command->cmd, F_OK) == 0 && access(command->cmd, X_OK) == -1)
			printerror(command->cmd);
        handlebuiltins(command);
        execve(command->cmd, command->args, command->env);
        if (!ft_find(command->args[0], "./"))
			error(command->args[0]);
		else
			perror(command->args[0]);
        exit(EXIT_FAILURE);
    }
    close(mini->pipes[command->index][1]);
    close(mini->pipes[command->index - 1][0]);
}

void handleiosingle(t_commandline *command)
{
    t_minishell *mini;

    mini = command->mini;
    if (command->infd == -2)
        command->infd = 0;
    if (command->outfd == -2 && command->next)
        command->outfd = mini->pipes[command->index][1];
    else if (command->outfd == -2 && !command->next)
        command->outfd = 1;
}

int checkcommand(t_commandline *command)
{
    if (!ft_strcmp(command->cmd, "cd") 
        || !ft_strcmp(command->cmd, "env") 
        || !ft_strcmp(command->cmd, "pwd")
        || !ft_strcmp(command->cmd, "echo")
        || !ft_strcmp(command->cmd, "export")
        || !ft_strcmp(command->cmd, "unset")
        || !ft_strcmp(command->cmd, "exit"))
        return (1);
    return (0);
}

void setupfirstcommand(t_commandline *command)
{
    pid_t pid;
    int size;
    t_minishell *mini;
    
    mini = command->mini;
    size = ft_commandsize(mini->commandline);
    command->env = constructenv(mini->env);
    pid = fork();
    if (!pid)
    {
        openfiles(command);
        handleiosingle(command);
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
        if (!ft_find(command->args[0], "./") && !checkcommand(command))
        {
            free(command->cmd);
            command->cmd = checkfile(command);
        }
        else
        {
            free(command->cmd);
            command->cmd = ft_strdup(command->args[0]);
        }
        if (access(command->cmd, F_OK) == 0 && access(command->cmd, X_OK) == -1)
			printerror(command->cmd);
        handlebuiltins(command);
        execve(command->cmd, command->args, command->env);
        if (!ft_find(command->args[0], "./"))
			error(command->args[0]);
		else
			perror(command->args[0]);
        exit(EXIT_FAILURE);
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
    int status;
    t_commandline *copy;

	i = -1;
    copy = command;
    initializepipes(command->mini);
    size = ft_commandsize(copy);
	while (++i < size)
    {
        childlabor(copy);
        copy = copy->next;
    }
	while (waitpid(-1, &status, 0) != -1)
        {}
    command->mini->exitstatus = status >> 8;
}

void execute(t_minishell *mini)
{
    // removenodes(mini);
    startpipex(mini->commandline);
}