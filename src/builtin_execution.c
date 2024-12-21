/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:40:53 by iskaraag          #+#    #+#             */
/*   Updated: 2024/12/17 16:45:57 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(char *args[])
{
	if (args[0] == NULL)
		return (1);
	if (ft_strncmp(args[0], "cd", 2) == 0)
	{
		handle_cd(args);
		return (2);
	}
	if (ft_strncmp(args[0], "env", 3) == 0)
	{
		handle_env();
		return (3);
	}
	if (ft_strncmp(args[0], "unset", 5) == 0)
	{
		handle_unset(args);
		return (4);
	}
	if (ft_strncmp(args[0], "export", 6) == 0)
	{
		handle_export(args);
		return (5);
	}
	return (0);
}
