/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:41:11 by iskaraag          #+#    #+#             */
/*   Updated: 2024/12/17 16:46:40 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input(t_data *data, char *args[])
{
	parse_command(data->input, args);
	if (args[0] == NULL)
	{
		free(data->input);
		return (0);
	}
	if (ft_strncmp(args[0], "exit", 4) == 0)
	{
		free(data->input);
		return (1);
	}
	return (0);
}
