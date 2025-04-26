/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 00:13:35 by aammisse          #+#    #+#             */
/*   Updated: 2025/04/26 17:18:47 by aammisse         ###   ########.fr       */
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
    infiles = copy->infile;
    while(infiles)
    {
        if (infiles->type != HEDOC)
            infd = open(infiles->file, O_RDONLY);
        infiles = infiles->next;
    }
    copy->lastinfd = infd;
    outfiles = copy->outfile;
    while(outfiles)
    {
        if (outfiles->type == REDOUT)
            outfd = open(outfiles->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
        else if (outfiles->type == APPEND)
            outfd = open(outfiles->file, O_RDWR | O_CREAT | O_APPEND, 0644);
        outfiles = outfiles->next;
    }
    copy->lastoutfd = outfd;
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

void	commandparse(t_commandline *command)
{
    if (command->lastinfd != STDIN_FILENO)
    {
        dup2(command->lastinfd, 0);
        close(command->lastinfd);
    }
    if (command->lastoutfd != STDOUT_FILENO)
    {
        dup2(command->lastoutfd, 1);
        close(command->lastoutfd);
    }
	if (!ft_find(command->args[0], "./"))
        command->cmd = checkfile(command);
	else
        command->cmd = ft_strdup(command->args[0]);
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

void getfds(t_commandline *command, int outfd)
{
    if (!command->index && !command->incheck)
        command->lastinfd = STDIN_FILENO;
    else if (!command->incheck && command->index)
        command->lastinfd = command->lastpipe;
    if (!command->index && !command->outcheck && command->next)
        command->lastoutfd = outfd;
    else if (command->index && !command->next && !command->outcheck)
        command->lastoutfd = STDOUT_FILENO;
}

void	childlabor(t_commandline *commandline)
{
    int		fd[2];
    pid_t   pid;
    
    pipe(fd);
    getfds(commandline, fd[1]);
    commandline->env = constructenv(commandline->mini->env);
	pid = fork();
    if (!pid)
    {
        openfiles(commandline);
        commandparse(commandline);
        // dup2(commandline->lastinfd, STDIN_FILENO);
        // close(commandline->lastinfd);
        // dup2(commandline->lastoutfd, STDOUT_FILENO);
        // close(commandline->lastoutfd);
        if (!ft_find(commandline->args[0], "./"))
            commandline->cmd = checkfile(commandline);
	    else
            commandline->cmd = ft_strdup(commandline->args[0]);
        execve(commandline->cmd, commandline->args, commandline->env);
        perror("execve");
        exit(0);
    }
    close(fd[1]);
    close(commandline->lastpipe);
    if (commandline->next)
        commandline->next->lastpipe = fd[0];
}

void	startpipex(t_commandline *commandline)
{
	int	i;
    int size;
    t_commandline *copy;

	i = -1;
    copy = commandline;
    size = ft_commandsize(copy);
	while (++i < size)
    {
        if (copy->lastinfd)
            copy->incheck = 1;
        if (copy->lastoutfd)
            copy->outcheck = 1;
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