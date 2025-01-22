/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:40:56 by iskaraag          #+#    #+#             */
/*   Updated: 2025/01/22 20:37:50 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int the_signal)
{
	if (the_signal == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		write(2, "\n", 1);
		rl_redisplay();
	}
}
