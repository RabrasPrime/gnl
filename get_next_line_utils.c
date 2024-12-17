/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjooris <tjooris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:20:37 by tjooris           #+#    #+#             */
/*   Updated: 2024/12/15 15:20:39 by tjooris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "get_next_line.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i + sizeof(ptrdiff_t) < n)
	{
		*(ptrdiff_t *)(dst + i) = *(ptrdiff_t *)(src + i);
		i += sizeof(ptrdiff_t);
	}
	while (i < n)
	{
		*(unsigned char *)(dst + i) = *(unsigned char *)(src + i);
		++i;
	}
	return (dst);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	if (dst <= src || dst >= src + len)
		ft_memcpy(dst, src, len);
	else
	{
		while (len >= sizeof(ptrdiff_t))
		{
			len -= sizeof(ptrdiff_t);
			*(ptrdiff_t *)(dst + len) = *(ptrdiff_t *)(src + len);
		}
		while (len > 0)
		{
			--len;
			*(unsigned char *)(dst + len) = *(unsigned char *)(src + len);
		}
	}
	return (dst);
}

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*cp;

	cp = (unsigned char *)s;
	c = (unsigned char)c;
	while (n--)
	{
		if (*cp == c)
			return (cp);
		++cp;
	}
	return (NULL);
}

char	*ft_strndup(char *ptr, size_t n)
{
	char	*line;

	line = malloc(n + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, ptr, n);
	line[n] = '\0';
	return (line);
}
