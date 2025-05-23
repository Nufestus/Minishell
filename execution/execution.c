/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 00:13:35 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/22 23:43:32 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	openfiles(t_commandline *command, t_minishell *mini)
{
	int			outfd;
	int			infd;
	char		*del;
	t_files		*infiles;
	t_files		*outfiles;

	outfd = -2;
	infd = -2;
	infiles = command->infile;
	while (infiles)
	{
		if (infiles->type != HEDOC)
			infd = open(infiles->file, O_RDONLY);
		else if (infiles->type == HEDOC)
		{
			del = ft_strdup(infiles->delimiter);
			infd = getinput(infiles->delinquotes, del, mini);
			free(del);
			if (infd == -4)
				return (1);
		}
		if (infd == -1)
			perror(infiles->file);
		if (infiles->next && infd != -1 && infd != -4)
			close(infd);
		infiles = infiles->next;
	}
	command->infd = infd;
	outfiles = command->outfile;
	while (outfiles)
	{
		if (outfiles->type == REDOUT)
			outfd = open(outfiles->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
		else if (outfiles->type == APPEND)
			outfd = open(outfiles->file, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (outfiles->next && outfd != -1)
			close(outfd);
		outfiles = outfiles->next;
	}
	command->outfd = outfd;
	return (0);
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

char	*my_getenv(t_minishell *mini, char *str)
{
	t_env	*env;

	env = mini->env;
	while (env)
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

int	is_directory(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf))
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

void	directoryerror(char *s)
{
	write(2, s, ft_strlen(s));
	write(2, ": Is a directory\n", 18);
	exit(126);
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
	if (!path)
		return (NULL);
	i = -1;
	while (path[++i])
	{
		str = ft_strjoin(path[i], "/");
		tmp = str;
		str = ft_strjoin(str, command->args[0]);
		free(tmp);
		if (!access(str, F_OK | X_OK))
			return (freestr(path), str);
		free(str);
	}
	return (freestr(path), NULL);
}

char	**constructenv(t_env *env)
{
	int		i;
	int		len;
	int		size;
	char	**res;
	t_env	*copy;

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

void	initializepipes(t_minishell *mini)
{
	int	size;
	int	i;

	i = 0;
	size = ft_commandsize(mini->commandline);
	mini->pipes = malloc((size) * sizeof(int *));
	while (i < size)
	{
		mini->pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(mini->pipes[i]) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

void	printerror(char *str)
{
	write(2, str, ft_strlen(str));
	write(2, ": Permission denied\n", 21);
	exit(126);
}

void	error(char *str)
{
	char	*tmp;

	tmp = ft_strjoin(str, ": command not found\n");
	write(2, tmp, ft_strlen(tmp) + 1);
	free(tmp);
}

void	handlebuiltins(t_commandline *command)
{
	if (command->cmd)
	{
		if (!ft_strcmp(command->cmd, "env"))
			ft_env(command->mini, command->args);
		else if (!ft_strcmp(command->cmd, "pwd"))
			ft_pwd(command->mini);
		else if (!ft_strcmp(command->cmd, "cd"))
			ft_cd(command);
		else if (!ft_strcmp(command->cmd, "echo"))
			ft_echo(command);
		else if (!ft_strcmp(command->cmd, "unset"))
			unset(command);
		else if (!ft_strcmp(command->cmd, "exit"))
			ft_exit(command);
		else if (!ft_strcmp(command->cmd, "export"))
			export(command);
	}
}

void	closeallfiles(t_minishell *mini)
{
	t_commandline	*copy;

	copy = mini->commandline;
	while (copy)
	{
		if (copy->outfd > 2)
			close(copy->outfd);
		if (copy->infd > 2)
			close(copy->infd);
		copy = copy->next;
	}
}

void	handleiolast(t_commandline *command)
{
	int			size;
	t_minishell	*mini;

	mini = command->mini;
	size = ft_commandsize(mini->commandline);
	if (command->infd == -1)
	{
		closeallpipes(mini, size);
		if (command->outfd != -1 && command->outfd != -2)
			close(command->outfd);
		exit(1);
	}
	if (command->infd == -2)
		command->infd = mini->pipes[command->index - 1][0];
	if (command->outfd == -2)
		command->outfd = 1;
}

void	setuplastcommand(t_commandline *command)
{
	int			size;
	pid_t		pid;
	t_minishell	*mini;

	mini = command->mini;
	size = ft_commandsize(mini->commandline);
	command->env = constructenv(mini->env);
	pid = fork();
	if (!pid)
	{
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
		closeallfiles(command->mini);
		closeallpipes(command->mini, size);
		if (command->cmd && command->cmd[0] == '\0')
		{
			error("\'\'");
			freedoublearray(command->args);
			free(command->cmd);
			exit(127);
		}
		else if (!command->cmd)
		{
			freedoublearray(command->args);
			exit(1);
		}
		if (!ft_find(command->args[0], "/")
			&& !checkcommand(command->cmd))
		{
			free(command->cmd);
			command->cmd = checkfile(command);
		}
		else
		{
			free(command->cmd);
			command->cmd = ft_strdup(command->args[0]);
		}
		if (access(command->cmd, F_OK) == 0 && access(command->cmd, X_OK) == -1
			&& !is_directory(command->cmd))
			printerror(command->cmd);
		else if ((is_directory(command->cmd)) && ((ft_find(command->args[0], ".")
			&& ft_find(command->args[0], "/")) || ft_find(command->args[0], "/")))
			directoryerror(command->cmd);
		handlebuiltins(command);
		execve(command->cmd, command->args, command->env);
		if (!ft_find(command->args[0], "/"))
		{
			error(command->args[0]);
			free(command->cmd);
			freedoublearray(command->args);
			exit(127);
		}
		else if (command->args)
		{
			perror(command->args[0]);
			exit(127);
		}
		free(command->cmd);
		freedoublearray(command->args);
		exit(1);
	}
}

void	handleiomiddle(t_commandline *command)
{
	int			size;
	t_minishell	*mini;

	mini = command->mini;
	size = ft_commandsize(mini->commandline);
	if (command->infd == -1)
	{
		closeallpipes(mini, size);
		exit(1);
	}
	if (command->infd == -2)
		command->infd = mini->pipes[command->index - 1][0];
	if (command->outfd == -2)
		command->outfd = mini->pipes[command->index][1];
}

void	setupmiddlecommand(t_commandline *command)
{
	int			size;
	pid_t		pid;
	t_minishell	*mini;

	mini = command->mini;
	size = ft_commandsize(mini->commandline);
	command->env = constructenv(mini->env);
	pid = fork();
	if (!pid)
	{
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
		closeallfiles(command->mini);
		closeallpipes(command->mini, size);
		if (command->cmd && command->cmd[0] == '\0')
		{
			error("\'\'");
			freedoublearray(command->args);
			free(command->cmd);
			exit(127);
		}
		else if (!command->cmd)
		{
			freedoublearray(command->args);
			exit(0);
		}
		if (!ft_find(command->args[0], "/")
			&& !checkcommand(command->cmd))
		{
			free(command->cmd);
			command->cmd = checkfile(command);
		}
		else
		{
			free(command->cmd);
			command->cmd = ft_strdup(command->args[0]);
		}
		if (access(command->cmd, F_OK) == 0 && access(command->cmd, X_OK) == -1 && !is_directory(command->cmd))
			printerror(command->cmd);
		else if ((is_directory(command->cmd)) && ((ft_find(command->args[0], ".") && ft_find(command->args[0], "/"))
				|| ft_find(command->args[0], "/")))
			directoryerror(command->cmd);
		handlebuiltins(command);
		execve(command->cmd, command->args, command->env);
		if (!ft_find(command->args[0], "/"))
		{
			error(command->args[0]);
			free(command->cmd);
			freedoublearray(command->args);
			exit(127);
		}
		else if (command->args)
		{
			perror(command->args[0]);
			exit(127);
		}
		free(command->cmd);
		freedoublearray(command->args);
		exit(1);
	}
}

int	checkcommand(char *cmd)
{
	if (cmd)
	{
		if (!ft_strcmp(cmd, "cd") 
			|| !ft_strcmp(cmd, "env") 
			|| !ft_strcmp(cmd, "pwd")
			|| !ft_strcmp(cmd, "echo")
			|| !ft_strcmp(cmd, "export")
			|| !ft_strcmp(cmd, "unset")
			|| !ft_strcmp(cmd, "exit"))
			return (1);
		return (0);
	}
	return (15);
}

static int	check_sign(int sign)
{
	if (sign < 0)
		return (0);
		else
		return (-1);
}

int	ft_atoi_custom(const char *str)
{
	int			sign;
	int			i;
	long long	result;
	long long	old_result;

	result = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		old_result = result;
		result = result * 10 + (sign * (str[i] - '0'));
		if ((result / 10) != old_result)
			return (check_sign(sign));
		i++;
	}
	return (result);
}

void	handleiosingle(t_commandline *command)
{
	int			size;
	t_minishell	*mini;

	mini = command->mini;
	size = ft_commandsize(mini->commandline);
	if (command->infd == -1)
	{
		closeallpipes(mini, size);
		if (command->outfd != -2 && command->outfd != -1)
			close(command->outfd);
		exit(1);
	}
	if (command->infd == -2)
		command->infd = 0;
	if (command->outfd == -2 && command->next)
		command->outfd = mini->pipes[command->index][1];
	else if (command->outfd == -2 && !command->next)
		command->outfd = 1;
}

void	setupfirstcommand(t_commandline *command)
{
	int			size;
	pid_t		pid;
	t_env		*node;
	t_minishell	*mini;
	
	mini = command->mini;
	size = ft_commandsize(mini->commandline);
	command->env = constructenv(mini->env);
	pid = fork();
	if (!pid)
	{
		handleiosingle(command);
		if (command->infd != STDIN_FILENO)
		{
			dup2(command->infd, STDIN_FILENO);
			close(command->infd);
		}
		if (command->outfd != STDOUT_FILENO)
		{
			dup2(command->outfd, STDOUT_FILENO);
			close(command->outfd);
		}
		closeallfiles(command->mini);
		closeallpipes(command->mini, size);
		if (command->cmd && command->cmd[0] == '\0')
		{
			error("\'\'");
			freedoublearray(command->args);
			free(command->cmd);
			exit(127);
		}
		else if (!command->cmd)
		{
			freedoublearray(command->args);
			exit(0);
		}
		if (!ft_find(command->args[0], "/") && !ft_find(command->args[0], ".")
			&& !checkcommand(command->cmd))
		{
			free(command->cmd);
			command->cmd = checkfile(command);
		}
		else
		{
			free(command->cmd);
			command->cmd = ft_strdup(command->args[0]);
		}
		if (access(command->cmd, F_OK) == 0 && access(command->cmd, X_OK) == -1 && !is_directory(command->cmd))
			printerror(command->cmd);
		else if ((is_directory(command->cmd)) && ((ft_find(command->args[0], ".") && ft_find(command->args[0], "/"))
				|| ft_find(command->args[0], "/")))
			directoryerror(command->cmd);
		handlebuiltins(command);
		if (!ft_strcmp(command->args[0], "./minishell"))
		{
			node = getenvnode(command->mini->env, "SHLVL");
			int num = ft_atoi_custom(node->value);
			num++;
			char *value = ft_itoa(num);
			free(node->value);
			node->value = ft_strdup(value);
			node->string = ft_strjoin(node->variable, "=");
			char *tmp = node->string;
			node->string = ft_strjoin(node->string, node->value);
			free(tmp);
			freedoublearray(command->env);
			command->env = constructenv(command->mini->env);
		}
		execve(command->cmd, command->args, command->env);
		if (command->args && !ft_find(command->args[0], "/"))
		{
			error(command->args[0]);
			free(command->cmd);
			freedoublearray(command->args);
			exit(127);
		}
		else if (command->args)
		{
			perror(command->args[0]);
			exit(127);
		}
		free(command->cmd);
		freedoublearray(command->args);
		exit(1);
	}
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

void	closeallpipes(t_minishell *mini, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		close(mini->pipes[i][0]);
		close(mini->pipes[i][1]);
		i++;
	}
}

void	startpipex(t_minishell *mini)
{
	int				i;
	int				size;
	int				status;
	t_commandline	*copy;

	i = -1;
	copy = mini->commandline;
	initializepipes(mini);
	size = ft_commandsize(copy);
	while (++i < size)
	{
		if (size > 1 || !checkcommand(copy->cmd))
		{
			if (mini->commandline->infd == -4)
			{
				copy = copy->next;
				continue;
			}
			childlabor(copy);
		}
		else if (size == 1)
		{
			if (mini->commandline->infd == -1)
			{
				copy = copy->next;
				continue;
			}
			handleiosingle(mini->commandline);
			closeallpipes(mini, size);
			mini->commandline->env = constructenv(mini->env);
			handlebuiltins(copy);
			return ;
		}
		copy = copy->next;
	}
	closeallpipes(mini, size);
	while (waitpid(-1, &status, 0) != -1)
		{}
	copy = mini->commandline;
	while (copy)
	{
		if (copy->outfd != -1 && copy->outfd != -2)
			close(copy->outfd);
		if (copy->infd != -1 && copy->infd != -2 && copy->infd != -4)
			close(copy->infd);
		copy = copy->next;
	}
	mini->exitstatus = WEXITSTATUS(status);
}

void	execute(t_minishell *mini)
{
	signal(SIGQUIT, signalhandle);
	startpipex(mini);
}
