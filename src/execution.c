/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:41:07 by iskaraag          #+#    #+#             */
/*   Updated: 2024/12/17 17:38:08 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(char ***commands, int count)
{
	int	i;

	i = 0;
	if (!commands)
		return ;
	while (i < count)
	{
		if (commands[i])
			free(commands[i]);
		i++;
	}
	free(commands);
}

void	execute_single_command(t_pipe_data *pdata, int i)
{
	char	*command_path;

	if (pdata->in_fd != 0)
	{
		if (dup2(pdata->in_fd, STDIN_FILENO) == -1)
			perror("dup2 input error");
		close(pdata->in_fd);
	}
	if (i < pdata->command_count - 1)
	{
		if (dup2(pdata->fd[1], STDOUT_FILENO) == -1)
			perror("dup2 output error");
	}
	close(pdata->fd[0]);
	close(pdata->fd[1]);
	if (!pdata->commands[i] || !pdata->commands[i][0])
	{
		write(2, "Invalid command\n", 16);
		exit(1);
	}
	command_path = find_command_in_path(pdata->commands[i][0]);
	if (!command_path)
	{
		write(2, pdata->commands[i][0], ft_strlen(pdata->commands[i][0]));
		write(2, "Command not found\n", 18);
		exit(1);
	}
	execve(command_path, pdata->commands[i], environ);
	perror("execve error");
	exit(1);
}

void	execute_piped_commands(char **args)
{
	t_pipe_data	pdata;
	int			i;

	i = 0;
	pdata.in_fd = 0;
	pdata.commands = malloc(MAX_ARGS * sizeof(char **));
	if (!pdata.commands)
	{
		perror("malloc failed");
		exit(1);
	}
	while (i < MAX_ARGS)
	{
		pdata.commands[i] = malloc(MAX_ARGS * sizeof(char *));
		if (!pdata.commands[i])
		{
			perror("malloc failed");
			exit(1);
		}
		i++;
	}
	pdata.command_count = parse_piped_commands(args, pdata.commands);
	if (pdata.command_count <= 0)
	{
		free_commands(pdata.commands, pdata.command_count);
		return ;
	}
	i = 0;
	while (i < pdata.command_count)
	{
		if (pipe(pdata.fd) == -1)
		{
			perror("pipe error");
			exit(1);
		}
		pdata.pid = fork();
		if (pdata.pid == -1)
		{
			perror("fork error");
			exit(1);
		}
		if (pdata.pid == 0)
		{
			execute_single_command(&pdata, i);
		}
		else
		{
			waitpid(pdata.pid, NULL, 0);
			close(pdata.fd[1]);
			if (pdata.in_fd != 0)
				close(pdata.in_fd);
			pdata.in_fd = pdata.fd[0];
		}
		i++;
	}
	if (pdata.in_fd != 0)
		close(pdata.in_fd);
	free_commands(pdata.commands, pdata.command_count);
}

void	execution(t_data *data, char *args[])
{
	int	builtin;

	if (contains_pipe(args))
	{
		execute_piped_commands(args);
		return ;
	}
	if (args[0][0] == '/' || ft_strchr(args[0], '/'))
	{
		if (access(args[0], X_OK) == 0)
			data->command_path = ft_strdup(args[0]);
		else
		{
			printf("%s: Command not found\n", args[0]);
			free(data->command_path);
			return ;
		}
	}
	else
		data->command_path = find_command_in_path(args[0]);
	builtin = execute_builtin(args);
	if (!data->command_path && builtin == 0)
	{
		printf("%s: Command not found\n", args[0]);
		free(data->command_path);
		return ;
	}
	data->pid = fork();
	if (data->pid == 0)
	{
		if (execve(data->command_path, args, NULL) == -1 && builtin == 0)
			perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (data->pid < 0)
		perror("fork error");
	else
		waitpid(data->pid, &data->status, 0);
	free(data->command_path);
}