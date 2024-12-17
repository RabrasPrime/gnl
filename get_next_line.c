/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjooris <tjooris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:20:29 by tjooris           #+#    #+#             */
/*   Updated: 2024/12/17 14:18:18 by tjooris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "get_next_line.h"

static int	reserve(t_buffer *buffer, size_t size)
{
	size_t	ncapacity;
	char	*reallocated;

	if (buffer->capacity - buffer->length < size)
	{
		if (buffer->capacity * 2 - buffer->length < size)
			ncapacity = buffer->length + size;
		else
			ncapacity = buffer->capacity * 2;
		reallocated = malloc(ncapacity);
		if (!reallocated)
			return (0);
		if (buffer->data)
			ft_memmove(reallocated, buffer->data, buffer->length);
		free(buffer->data);
		buffer->data = reallocated;
		buffer->capacity = ncapacity;
	}
	return (1);
}

static int	read_un_nl(int fd, t_buffer *buffer, char **eol_pos)
{
	size_t	eol;
	ssize_t	count;

	eol = 0;
	count = BUFFER_SIZE;
	while (1)
	{
		*eol_pos = ft_memchr(buffer->data + eol, '\n',
				buffer->length - eol);
		if (*eol_pos != NULL || count != BUFFER_SIZE)
			break ;
		eol = buffer->length;
		if (!reserve(buffer, BUFFER_SIZE))
			return (-1);
		count = read(fd, buffer->data + buffer->length, BUFFER_SIZE);
		if (count == -1)
			break ;
		buffer->length += count;
	}
	return (count);
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
	read_count = read_un_nl(fd, &buffer, &eol_pos);
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
