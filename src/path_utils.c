/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:41:16 by iskaraag          #+#    #+#             */
/*   Updated: 2024/12/17 16:55:38 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	build_path(char *full_path, size_t size, const char *token,
		const char *command)
{
	full_path[0] = '\0';
	if (ft_strlen(token) < size)
	{
		ft_strlcpy(full_path, token, size - 1);
		full_path[size - 1] = '\0';
	}
	if (ft_strlen(full_path) + 1 < size)
	{
		ft_strlcat(full_path, "/", size - ft_strlen(full_path) - 1);
	}
	if (ft_strlen(full_path) + ft_strlen(command) < size)
	{
		ft_strlcat(full_path, command, size - ft_strlen(full_path) - 1);
	}
}

char	*find_command_in_path(const char *command)
{
	t_data	data;
	char	full_path[1024];

	data.path = getenv("PATH");
	if (!data.path)
		return (NULL);
	data.path_copy = ft_strdup(data.path);
	if (!data.path_copy)
		return (perror("Memory allocation failed"), NULL);
	data.token = ft_strtok(data.path_copy, ":");
	while (data.token)
	{
		build_path(full_path, sizeof(full_path), data.token, command);
		if (access(full_path, X_OK) == 0)
		{
			free(data.path_copy);
			return (ft_strdup(full_path));
		}
		data.token = ft_strtok(NULL, ":");
	}
	free(data.path_copy);
	return (NULL);
}

char	*ft_strchr(const char *s, int c)
{
	int	a;

	a = 0;
	while (s[a] != '\0')
	{
		if (s[a] == (unsigned char)c)
			return ((char *)(s + a));
		a++;
	}
	if (s[a] == (unsigned char)c)
		return ((char *)(s + a));
	return (0);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*next_token = NULL;
	char		*token_start;

	if (str != NULL)
		next_token = str;
	if (next_token == NULL || *next_token == '\0')
		return (NULL);
	while (*next_token != '\0' && ft_strchr(delim, *next_token) != NULL)
		next_token++;
	if (*next_token == '\0')
		return (NULL);
	token_start = next_token;
	while (*next_token != '\0' && ft_strchr(delim, *next_token) == NULL)
		next_token++;
	if (*next_token != '\0')
	{
		*next_token = '\0';
		next_token++;
	}
	return (token_start);
}
