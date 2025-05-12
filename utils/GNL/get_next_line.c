/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 13:06:05 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/05 13:24:51 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == c)
		return ((char *)s);
	else
		return (NULL);
}

char	*read_file(int fd, char *str)
{
	char	*buffer;
	char	*tmp;
	ssize_t	bytes;

	buffer = (char *)ft_calloc((size_t)BUFFER_SIZE + 1, 1);
	bytes = 1;
	while (!ft_strchr(str, '\n') && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
		{
			free(buffer);
			free(str);
			return (NULL);
		}
		buffer[bytes] = '\0';
		tmp = str;
		str = ft_stringjoin(str, buffer);
		free(tmp);
	}
	free(buffer);
	return (str);
}

char	*read_line(char *str)
{
	char	*new_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!*str || !str)
		return (NULL);
	while (str[i] != '\n' && str[i])
		i++;
	if (str[i] == '\n')
		i++;
	new_str = (char *)ft_calloc(i + 1, 1);
	while (j < i)
	{
		new_str[j] = str[j];
		j++;
	}
	new_str[j] = '\0';
	return (new_str);
}

char	*get_next_line(int fd)
{
	static char		*str;
	char			*line;

	str = NULL;
	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	str = read_file(fd, str);
	if (!str)
		return (NULL);
	line = read_line(str);
	free(str);
	return (line);
}
