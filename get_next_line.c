/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjooris <tjooris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:20:29 by tjooris           #+#    #+#             */
/*   Updated: 2024/12/15 15:21:17 by tjooris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "get_next_line.h"

static bool	reserve(t_buffer *buffer, size_t size)
{
	size_t	new_capacity;
	char	*reallocated;

	if (buffer->capacity - buffer->length < size)
	{
		if (buffer->capacity * 2 - buffer->length < size)
			new_capacity = buffer->length + size;
		else
			new_capacity = buffer->capacity * 2;
		reallocated = malloc(new_capacity);
		if (reallocated == NULL)
			return (false);
		if (buffer->data)
			ft_memmove(reallocated, buffer->data, buffer->length);
		free(buffer->data);
		buffer->data = reallocated;
		buffer->capacity = new_capacity;
	}
	return (true);
}

static int	read_until_nl(int fd, t_buffer *buffer, char **eol_pos)
{
	size_t	eol_start;
	ssize_t	read_count;

	eol_start = 0;
	read_count = BUFFER_SIZE;
	while (true)
	{
		*eol_pos = ft_memchr(buffer->data + eol_start, '\n',
				buffer->length - eol_start);
		if (*eol_pos != NULL || read_count != BUFFER_SIZE)
			break ;
		eol_start = buffer->length;
		if (!reserve(buffer, BUFFER_SIZE))
			return (-1);
		read_count = read(fd, buffer->data + buffer->length, BUFFER_SIZE);
		if (read_count == -1)
			break ;
		buffer->length += read_count;
	}
	return (read_count);
}

static char	*get_line(t_buffer *buffer, char *eol_pos)
{
	size_t	line_length;
	char	*line;

	line_length = 0;
	if (eol_pos != NULL)
		line_length = eol_pos - buffer->data + 1;
	else if (buffer->length > 0)
		line_length = buffer->length;
	line = ft_strndup(buffer->data, line_length);
	if (!line)
		return (NULL);
	buffer->length -= line_length;
	ft_memmove(buffer->data, buffer->data + line_length, buffer->length);
	return (line);
}

char	*get_next_line(int fd)
{
	static t_buffer	buffer;
	ssize_t			read_count;
	char			*eol_pos;
	char			*line;

	line = NULL;
	read_count = read_until_nl(fd, &buffer, &eol_pos);
	if (read_count != -1 && buffer.length > 0)
		line = get_line(&buffer, eol_pos);
	if (read_count <= 0 || line == NULL)
	{
		free(buffer.data);
		buffer.data = NULL;
		buffer.length = 0;
		buffer.capacity = 0;
		if (read_count == -1 || line == NULL)
		{
			free(line);
			line = NULL;
			return (NULL);
		}
	}
	return (line);
}
