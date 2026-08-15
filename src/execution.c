/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:41:07 by iskaraag          #+#    #+#             */
/*   Updated: 2025/01/13 19:00:07 by iskaraag         ###   ########.fr       */
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
	t_drct_data	d_data;
	int			builtin_status;
	int			i;

	d_data.input_redirect = 0;
	d_data.output_redirect = 0;
	d_data.append_mode = 0;
	d_data.input_file = NULL;
	d_data.output_file = NULL;
	builtin_status = 0;
	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "<\0", 2) == 0)
		{
			d_data.input_redirect = 1;
			d_data.input_file = args[i + 1];
			args[i] = NULL;
		}
		else if (ft_strncmp(args[i], ">\0", 2) == 0)
		{
			d_data.output_redirect = 1;
			d_data.append_mode = 0;
			d_data.output_file = args[i + 1];
			args[i] = NULL;
		}
		else if (ft_strncmp(args[i], ">>\0", 3) == 0)
		{
			d_data.output_redirect = 1;
			d_data.append_mode = 1;
			d_data.output_file = args[i + 1];
			args[i] = NULL;
		}
		i++;
	}
	if (args[0] == NULL)
	{
		printf("Invalid command\n");
		return ;
	}
	builtin_status = execute_builtin(args);
	if (builtin_status != 0)
		return ;
	if (contains_pipe(args))
	{
		execute_piped_commands(args);
		return ;
	}
	if (args[0][0] == '/' || ft_strchr(args[0], '/'))
	{
		if (access(args[0], X_OK) == 0)
		{
			data->command_path = ft_strdup(args[0]);
		}
		else
		{
			printf("%s: Command not found\n", args[0]);
			return ;
		}
	}
	else
	{
		data->command_path = find_command_in_path(args[0]);
	}
	if (!data->command_path)
	{
		printf("%s: Command not found\n", args[0]);
		return ;
	}
	data->pid = fork();
	if (data->pid == 0)
	{
		if (d_data.input_redirect && d_data.input_file)
		{
			d_data.fd = open(d_data.input_file, O_RDONLY);
			if (d_data.fd < 0)
			{
				perror("Error opening input file");
				exit(EXIT_FAILURE);
			}
			dup2(d_data.fd, STDIN_FILENO);
			close(d_data.fd);
		}
		if (d_data.output_redirect && d_data.output_file)
		{
			if (d_data.append_mode)
			{
				d_data.fd = open(d_data.output_file,
						O_WRONLY | O_CREAT | O_APPEND, 0644);
			}
			else
			{
				d_data.fd = open(d_data.output_file,
						O_WRONLY | O_CREAT | O_TRUNC, 0644);
			}
			if (d_data.fd < 0)
			{
				perror("Error opening output file");
				exit(EXIT_FAILURE);
			}
			dup2(d_data.fd, STDOUT_FILENO);
			close(d_data.fd);
		}
		if (execve(data->command_path, args, environ) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else if (data->pid < 0)
	{
		perror("Fork failed");
	}
	else
	{
		waitpid(data->pid, &data->status, 0);
	}
	free(data->command_path);
}

/*
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
*/
