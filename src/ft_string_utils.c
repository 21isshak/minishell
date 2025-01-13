/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iskaraag <iskaraag@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:41:09 by iskaraag          #+#    #+#             */
/*   Updated: 2025/01/13 19:00:16 by iskaraag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	c;

	c = 0;
	while (str[c] != '\0')
	{
		c++;
	}
	return (c);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size != 0)
	{
		while (src[i] != 0 && i < (size - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (ft_strlen(src));
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	a;
	size_t	b;

	a = 0;
	b = 0;
	while (dst[a] != '\0' && a < size)
		a++;
	while (src[b] != '\0' && (a + b + 1) < size)
	{
		dst[a + b] = src[b];
		b++;
	}
	if (a < size)
		dst[a + b] = '\0';
	return (a + ft_strlen(src));
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	t;
	size_t	f;

	t = 0;
	f = 0;
	while (t < n && s1[t] != '\0' && s2[f] != '\0')
	{
		if (s1[t] != s2[f])
			return (s1[t] - s2[f]);
		t++;
		f++;
	}
	if (t < n)
		return (s1[t] - s2[f]);
	return (0);
}

char	*ft_strdup(const char *src)
{
	size_t	ln;
	size_t	i;
	char	*dest;

	ln = 0;
	while (src[ln] != '\0')
	{
		ln++;
	}
	dest = (char *)malloc(sizeof(char) * (ln + 1));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
		write (fd, s, ft_strlen(s));
}

void	ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}
