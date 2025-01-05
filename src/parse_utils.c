/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:41:14 by iskaraag          #+#    #+#             */
/*   Updated: 2024/12/22 15:55:06 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	contains_pipe(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		if (ft_strncmp(args[i], "|", 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	parse_piped_commands(char **args, char ***commands)
{
	int	command_count;
	int	arg_index;
	int	cmd_index;
	int	i;
	int	j;

	i = -1;
	j = -1;
	command_count = 0;
	arg_index = 0;
	cmd_index = 0;
	while (i++ < MAX_ARGS)
	{
		while (j++ < MAX_ARGS)
			commands[i][j] = NULL;
	}
	while (args[arg_index] != NULL)
	{
		if (ft_strncmp(args[arg_index], "|", 1) == 0)
		{
			commands[command_count][cmd_index] = NULL;
			command_count++;
			cmd_index = 0;
		}
		else
			commands[command_count][cmd_index++] = args[arg_index];
		arg_index++;
	}
	commands[command_count][cmd_index] = NULL;
	commands[command_count + 1] = NULL;
	return (command_count + 1);
}

void	parse_command(char *input, char **args)
{
	char	*token;
	int		index;

	index = 0;
	token = ft_strtok(input, " \n");
	while (token != NULL && index < MAX_ARGS - 1)
	{
		args[index++] = token;
		token = ft_strtok(NULL, " \n");
	}
	args[index] = NULL;
}
