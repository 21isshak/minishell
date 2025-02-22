/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:40:53 by iskaraag          #+#    #+#             */
/*   Updated: 2025/01/20 18:10:46 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(char *args[])
{
	t_data	data;

	if (args[0] == NULL)
		return (1);
	if (ft_strncmp(args[0], "cd\0", 2) == 0)
	{
		handle_cd(args);
		return (2);
	}
	if (ft_strncmp(args[0], "env\0", 4) == 0)
	{
		handle_env();
		return (3);
	}
	if (ft_strncmp(args[0], "unset\0", 6) == 0)
	{
		handle_unset(args);
		return (4);
	}
	if (ft_strncmp(args[0], "export\0", 7) == 0)
	{
		handle_export(args);
		return (5);
	}
	if (ft_strncmp(args[0], "echo\0", 5) == 0)
	{
		handle_echo(args, data.status);
		return (6);
	}
	if (ft_strncmp(args[0], "clear\0", 6) == 0)
	{
		handle_clear();
		return (7);
	}
	return (0);
}
