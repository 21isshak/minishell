/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handlers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:41:05 by iskaraag          #+#    #+#             */
/*   Updated: 2025/01/13 18:58:53 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_cd(char *args[])
{
	char	*home;

	home = getenv("HOME");
	if (args[1] == NULL)
	{
		if (home)
			chdir(home);
		else
			perror("cd");
	}
	else
	{
		if (chdir(args[1]) != 0)
			perror("cd");
	}
}

void	handle_env(void)
{
	char	**env;

	env = environ;
	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

void	handle_unset(char *args[])
{
	int	i;

	if (args[1] == NULL)
	{
		write(2, "unset: missing arguments\n", 25);
		return ;
	}
	i = 1;
	while (args[i])
	{
		if (unsetenv(args[i]) != 0)
			perror("unset");
		i++;
	}
}

void	handle_export(char *args[])
{
	int		i;
	char	*equal;

	if (args[1] == NULL)
	{
		write(2, "export: missing arguments\n", 26);
		return ;
	}
	i = 1;
	while (args[i])
	{
		equal = ft_strchr(args[i], '=');
		if (equal)
		{
			*equal = '\0';
			if (setenv(args[i], equal + 1, 1) != 0)
				perror("export");
		}
		else
			write(2, "export: invalid format\n", 23);
		i++;
	}
}

void	handle_echo(char *args[])
{
	int		i;
	bool	newline;
	bool	in_quotes;
	char	quote_char;

	i = 1;
	newline = true;
	if (args[i] && !ft_strncmp(args[i], "-n\0", 3))
	{
		newline = false;
		i++;
	}
	while (args[i])
	{
		in_quotes = false;
		quote_char = '\0';
		while (*args[i])
		{
			if ((args[i][0] == '"' || args[i][0] == '\'') && !in_quotes)
			{
				in_quotes = true;
				quote_char = args[i][0];
				args[i]++;
			}
			if (in_quotes && *args[i] == quote_char)
			{
				in_quotes = false;
				args[i]++;
				break ;
			}
			write(1, args[i], 1);
			args[i]++;
		}
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
}

void	handle_clear(void)
{
	printf("\033[H\033[J");
}
