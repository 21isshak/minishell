/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:39:06 by iskaraag          #+#    #+#             */
/*   Updated: 2025/01/22 20:43:56 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av)
{
	t_data	data;
	char	*args[MAX_ARGS];

	(void)av;
	if (ac != 1)
		return (printf("\033[0;31mWRONG USAGE!\033[0m\nTRY: ./minishell\n"), 1);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_signal);
		data.input = readline("minishell> ");
		if (!data.input)
			break ;
		if (*data.input)
		{
			add_history(data.input);
			if (handle_input(&data, args))
				break ;
			execution(&data, args);
		}
		free(data.input);
	}
	rl_clear_history();
	return (0);
}
